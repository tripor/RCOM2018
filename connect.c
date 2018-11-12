#include "connect.h"
#include "stateMachine.h"

struct termios *oldtio,*newtio;
int connect_count=1;
int connect_fd=0;
int connect_alarm=0;
/**
 * @brief Funcao que e chamada quando o sinal do relogio e recebido. Só é chamada quando o programa esta a tentar estabelecer a conecao.
 * 
 */
void touchConnectSender()
{
    connect_alarm=1;
    printf("Timeout number %d in SET message response. Resending message...\n", connect_count);
    connect_count++;
    if(connect_count>=4)
    {
        printf("Couldn't make connection. Exiting...\n");
        exit(1);
    }
}
/**
 * @brief Estabelece a conecao. Deve ser feita por parte do Recetor
 * 
 * @param fd Descritor do ficheiro
 */
void makeConnectionReceiver(int fd)
{
    int res;
    unsigned char receive;
    connect_fd=fd;
    connect_count=0;
    //esperar pela mensagem de set do emissor
    printf("Waiting for the SET message from Sender...\n");
    while(getStateSet()!=5){
      res=read(fd,&receive,1);
      if(res==0)continue;
      stateMachineSET(receive);
    }
    //Mandar a mensagem de UA
    printf("SET message received. Sending UA message to Receiver...\n");
    sendMessage("UA","R",fd);
    printf("UA message sent to Receiver.\n");
}
/**
 * @brief Estabelece a conecao. deve ser feita por parte do Emissor
 * 
 * @param fd Descritor do ficheiro
 */
void makeConnectionSender(int fd)
{
    signal(SIGALRM, touchConnectSender);
    int res;
    unsigned char receive;
    connect_fd=fd;
    connect_count=0;
    //Mandar mensagem de SET
    printf("Sending SET message to Receiver...\n");
    sendMessage("SET","W",fd);
    printf("SET message sent to Receiver. Waiting for response...\n");
    //Esperar pela resposta do recetor
    alarm(WAITTIME);
    while(getStateUa()!=5){
        res=read(fd,&receive,1);
        if(connect_alarm)
        {
            signal(SIGALRM, touchConnectSender);
            sendMessage("SET","W",fd);
            connect_alarm=0;
            resetStates();
            alarm(WAITTIME);
            continue;
        }
        if(res==0) continue;
        stateMachineUA(receive);
    }
    connect_alarm=0;
    alarm(0);
    printf("UA message received from Receiver.\n");

}
/**
 * @brief Abre a conecao entre o emissor e o recetor. É necessario que a funcao setRead ou setWrite seja chamada antes para identificar o tipo.
 * 
 * @param canal Path com o canal a usar. Normal é o /dev/ttyS0
 * @return int Retorna o descrito do ficheiro correspondente ao canal
 */
int llOpen( char *canal)
{
  int fd;

  oldtio=(struct termios *)malloc(sizeof(struct termios));
  newtio=(struct termios *)malloc(sizeof(struct termios));
  // Open port to read
  fd = open(canal, O_RDWR | O_NOCTTY );

  if (fd <0) {perror(canal); exit(-1); }

  if ( tcgetattr(fd,oldtio) == -1) { /* save current port settings */
    perror("tcgetattr");
    exit(-1);
  }
  bzero(newtio, sizeof(*newtio));
  (*newtio).c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
  (*newtio).c_iflag = IGNPAR;
  (*newtio).c_oflag = 0;

  /* set input mode (non-canonical, no echo,...) */
  (*newtio).c_lflag = 0;

  (*newtio).c_cc[VTIME]    = 0;
  (*newtio).c_cc[VMIN]     = 1;
  tcflush(fd, TCIOFLUSH);

  if ( tcsetattr(fd,TCSANOW,newtio) == -1) {
    perror("tcsetattr");
    exit(-1);
  }

  printf("New termios structure set\n");

  if(getType())
  {
    makeConnectionReceiver(fd);
  }
  else
  {
    makeConnectionSender(fd);
  }

  return fd;
}
/**
 * @brief Aplica a estrutura termios antiga. Função auxiliar do llClose
 * 
 * @param fd Descritor do ficheiro do canal
 */
void closeL(int fd)
{
  tcsetattr(fd,TCSANOW,oldtio);
}
