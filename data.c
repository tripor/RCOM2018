#include "data.h"
#include "stateMachine.h"

void changestate2Read(unsigned char message, unsigned char bcc);
void changestate2Write(unsigned char message);
void changestate2WriteREJ(unsigned char message);

int s = 0;
int state2 = 0;
int state3 = 0;
unsigned char last_tram[10 * PackageSize];
int ja_last=0;

int count2 = 1;
int data_alarm = 0;
/**
 * @brief Funcao que trata do alarm quando e recebido este sinal. Esta função é só chamada quando estão a ser transferidos dados
 * 
 */
void touch2()
{
  data_alarm = 1;
  //printf("Timeout number %d in Data message response. Resending data...\n", count2);
  count2++;
  if (count2 >= 4)
  {
    //printf("Couldn't send data. Exiting...\n");
    exit(1);
  }
}
/**
 * @brief Compara um array de unsigned com o ultimo tram recebido. Caso ainda não haja um ultimo tram, este passa a ser esse
 * 
 * @param n1 Um array de unsigned chars
 * @param lenght Tamanho do array
 * @return int retorna 1 caso seja diferente ou 0 caso seja igual
 */
int comparer(unsigned char *n1,int lenght)
{
  if(ja_last)
  {
    int igual=0;
    for(int i=0;i<lenght;i++)
    {
      if(n1[i]!=last_tram[i])
      {
        igual=1;
      }
      last_tram[i]=n1[i];
    }
    return igual;
  }
  else
  {
    ja_last=1;
    for(int i=0;i<lenght;i++)
    {
      last_tram[i]=n1[i];
    }
    return 1;
  }
}

/**
 * @brief Recebe um byte de unsigned char e tranforma-o caso seja necessario. Stuffing
 * 
 * @param data Byte que vai sofrer stuffing
 * @param fd Descritor do ficheiro
 */
void writeStuff(unsigned char data, int fd)
{
  unsigned char send;
  if (data == FLAG)
  {
    send = ESCAPE;
    writeByte(fd, send);
    send = ESCAPEF;
    writeByte(fd, send);
  }
  else if (data == ESCAPE)
  {
    send = ESCAPE;
    writeByte(fd, send);
    send = ESCAPEE;
    writeByte(fd, send);
  }
  else
  {
    send = data;
    writeByte(fd, send);
  }
}
/**
 * @brief Verifica se um byte sofrer stuffing e retorna este byte
 * 
 * @param first Primeiro byte a verificar
 * @param second Segundo byte a verificar
 * @return unsigned char Retorna 0 caso não haja stuffing ou diferente caso haja stuffing
 */
unsigned char unStuff(unsigned char first, unsigned char second)
{
  if (first == ESCAPE && second == ESCAPEF)
  {
    return 0x7e;
  }
  else if (first == ESCAPE && second == ESCAPEE)
  {
    return ESCAPE;
  }
  else
    return 0x00;
}
/**
 * @brief Manda a trama com os campos de controlos
 * 
 * @param data Informação que eu quero enviar
 * @param length Tamnho da trama a enviar
 * @param fd Descritor do ficheiro
 */
void sendDataAux(unsigned char *data, int length, int fd)
{
  unsigned char bcc1 = 0, bcc2 = 0;
  unsigned char send;
  bcc1 ^= Aemi;
  if (s == 0)
    bcc1 ^= S0;
  else
    bcc1 ^= S1;
  for (int i = 0; i < length; i++)
  {
    bcc2 ^= data[i];
  }

  send = FLAG;
  writeByte(fd, send);
  send = Aemi;
  writeByte(fd, send);
  if (s == 0)
  {
    send = S0;
  }
  else
  {
    send = S1;
  }
  writeByte(fd, send);

  writeStuff(bcc1, fd);

  for (int i = 0; i < length; i++)
  {
    writeStuff(data[i], fd);
  }

  writeStuff(bcc2, fd);
  send = FLAG;
  writeByte(fd, send);
}
/**
 * @brief Escreve a trama de informacao no descritor. Espera por uma resposta por parte do recetor. Caso haja timeout ou a mensagem seja mal recebida volta a enviar
 * 
 * @param fd Descritor do ficheiro
 * @param data Informacao que eu pretendo enviar
 * @param length Comprimento da informacao
 * @return int Comprimento enviado. Deve ser sempre length, ou o programa termina a meio
 */
int llwrite(int fd, unsigned char *data, int length)
{
  int res;
  unsigned char receive;
  data_alarm = 0;
  state2 = 0;
  state3 = 0;
  sendDataAux(data, length, fd);
  signal(SIGALRM, touch2);
  //printf("Data sent. Waiting for the response...\n");
  alarm(WAITTIME);
  while (state2 != 5)
  {
    signal(SIGALRM, touch2);
    res = read(fd, &receive, 1);
    if (data_alarm)
    {
      data_alarm = 0;
      sendDataAux(data, length, fd);
      state2 = 0;
      state3 = 0;
      resetStates();
      alarm(WAITTIME);
      continue;
    }
    if (res == 0)
      continue;
    changestate2Write(receive);
    changestate2WriteREJ(receive);
    if (state3 == 5)
    {
      //printf("REJ message received ...\n");
      //printf("Resending data...\n");
      sendDataAux(data, length, fd);
      //printf("Data resent.\n");
      state2 = 0;
      state3 = 0;
      count2 = 1;
      alarm(WAITTIME);
      data_alarm = 0;
    }
  }
  alarm(0);
  //printf("Response received.\n");
  count2 = 1;
  if (s == 0)
    s = 1;
  else
    s = 0;
  return length;
}
/**
 * @brief Le informacao. Caso a informacao recebida esteja correta manda um mensagem de retorno de ready, caso contrario manda uma mensagem rej.
 * 
 * @param fd Descritor do ficheiro
 * @param guardar2 Onde eu vou guardar a informacao. Ja deve vir previamente alocada memoria suficiente, do tamanho de Packat size.
 * @return int Retorna o comprimento da informacao lida
 */
int llRead(int fd, unsigned char *guardar2)
{
  unsigned int bcc = 0, i = 0;
  unsigned char receive;
  unsigned char guardar[10 * PackageSize];
  state2 = 0;
  int res;
  int passei = 0;
  int k = 0;
  //printf("Reading data...\n");
  while (state2 != 6)
  {
    bcc = 0;
    i = 0;
    k = 0;
    res = read(fd, &receive, 1);
    if (res == 0)
      continue;
    changestate2Read(receive, 0);
    passei++;
    if (state2 != 0)
    {
      //Caso eu esteja a ler o primeiro byte de FLAG varias vezes, vou rejeitar este por ser o FLAG final da trama
      if (passei != 1)
      {
        passei = 0;
        continue;
      }
      passei = 0;
      receive = 0;
      // Receber todos os bytes da trama
      while (receive != FLAG && i <= MaximumRead)
      {
        res = read(fd, &receive, 1);
        if (res == 0)
          continue;
        guardar[i] = receive;
        i++;
      }
      // Caso a trama tenho um tamanho muito pequeno, vai ser descartada
      if (i < 4)
      {
        //printf("Error Reading...\n");
        state2 = 0;
        //printf("Sending REJ message to Sender, Flag error...\n");
        if (s == 0)
          sendMessage("REJ0", "R", fd);
        else
          sendMessage("REJ1", "R", fd);
        //printf("REJ message sent. S: %d\n", s);
        continue;
      }
      // Caso a trama tenha um tamanho demasiado grande
      if (i > MaximumRead)
        continue;
      i = i - 3;
      bcc = 0;           
      bcc ^= guardar[0]; 
      bcc ^= guardar[1]; 
      int j = 3, help = 0;
      //Verificar se o bcc não esta stuffed
      if (unStuff(guardar[2], guardar[3]) != 0)
        j++;
      if (unStuff(guardar[i], guardar[i + 1]) != 0)
      {
        i--;
        help++;
      }
      changestate2Read(guardar[0], bcc);
      changestate2Read(guardar[1], bcc);
      if(state2!=3)continue;
      if (unStuff(guardar[2], guardar[3]) == 0)
        changestate2Read(guardar[2], bcc);
      else
        changestate2Read(unStuff(guardar[2], guardar[3]), bcc);
      bcc = 0;
      //Guardar a informacao na variavel de parametro
      for (; j <= i; j++, k++)
      {
        unsigned int un = unStuff(guardar[j], guardar[j + 1]);
        if (j != i)
        {
          if (un == 0)
          {
            bcc ^= guardar[j];
            guardar2[k] = guardar[j];
          }
          else
          {
            bcc ^= un;
            guardar2[k] = un;
            j++;
          }
        }
        else
        {
          bcc ^= guardar[j];
          guardar2[k] = guardar[j];
        }
      }

      // Verificar se a trama recebida não é repetida
      if(comparer(guardar2,k)==0)
      {
        //printf("Repeated message. Sending RR...\n");
        if (s == 0)
          sendMessage("RR0", "R", fd);
        else
          sendMessage("RR1", "R", fd);
        //printf("RR message sent. \n");
        continue;
      }

      // Unstuff aos bytes finais
      if (unStuff(guardar[i + help], guardar[i + 1 + help]) == 0)
        changestate2Read(guardar[i + 1 + help], bcc);
      else
        changestate2Read(unStuff(guardar[i + help], guardar[i + 1 + help]), bcc);
      changestate2Read(guardar[i + 2 + help], bcc);
      // Caso a mensagem recebida esteja errada
      if (state2 != 6)
      {
        //printf("Sending REJ message to Sender. Message wrong...\n");
        if (s == 0)
          sendMessage("REJ0", "R", fd);
        else
          sendMessage("REJ1", "R", fd);
        //printf("REJ message sent. S:%d\n", s);
        state2 = 0;
      }
    }
  }
  if (s)
    s = 0;
  else
    s = 1;
  // Caso a mensagem recebida esteja correta
  //printf("Data read. Sending RR message to Sender...\n");
  if (s == 0)
    sendMessage("RR0", "R", fd);
  else
    sendMessage("RR1", "R", fd);
  //printf("RR message sent. \n");
  return k;
}
/**
 * @brief Maquina de estados para o recetor. Caso estado 6, a mensagem recebida esta correta
 * 
 * @param message Byte que eu recebi
 * @param bcc Bcc caso esteja no estado 3 ou 4
 */
void changestate2Read(unsigned char message, unsigned char bcc)
{
  switch (state2)
  {
  case 0:
    if (message == FLAG)
      state2 = 1;
    break;

  case 1:
    if (message == Aemi)
      state2 = 2;
    else if (message != FLAG)
      state2 = 0;
    break;

  case 2:
    if (message == S0 && s == 0)
    {
      state2 = 3;
    }
    else if (message == S1 && s == 1)
    {
      state2 = 3;
    }
    else if (message == FLAG)
      state2 = 1;
    else
      state2 = 0;
    break;
  case 3:
    if (message == (bcc))
      state2 = 4;
    else if (message == FLAG)
      state2 = 1;
    else
      state2 = 0;
    break;
  case 4:
    if (message == (bcc))
      state2 = 5;
    else if (message == FLAG)
      state2 = 1;
    else
      state2 = 0;
    break;
  case 5:
    if (message == FLAG)
      state2 = 6;
    else
      state2 = 0;

    break;
  default:
    state2 = 0;
  }
}
/**
 * @brief Maquina de estados para a mensagem RR. Se no estado 5, a mensagem RR foi recebida com sucesso
 * 
 * @param message Byte que eu recebi
 */
void changestate2Write(unsigned char message)
{
  switch (state2)
  {
  case 0:
    if (message == FLAG)
      state2 = 1;
    break;

  case 1:
    if (message == Arec)
      state2 = 2;
    else if (message != FLAG)
      state2 = 0;
    break;

  case 2:
    if (message == Crr0 && s == 1)
    {
      state2 = 3;
    }
    else if (message == Crr1 && s == 0)
    {
      state2 = 3;
    }
    else if (message == FLAG)
      state2 = 1;
    else
      state2 = 0;
    break;
  case 3:
    if ((message == (Arec ^ Crr0) && s == 1) || (message == (Arec ^ Crr1) && s == 0))
      state2 = 4;
    else if (message == FLAG)
      state2 = 1;
    else
      state2 = 0;
    break;
  case 4:
    if (message == FLAG)
      state2 = 5;
    else
      state2 = 0;

    break;
  default:
    state2 = 0;
  }
}
/**
 * @brief Maquina de estados para a mensagem REJ. Caso estado 5 a mensagem REJ foi recebida com sucesso
 * 
 * @param message Byte que eu recebi
 */
void changestate2WriteREJ(unsigned char message)
{
  switch (state3)
  {
  case 0:
    if (message == FLAG)
      state3 = 1;
    break;

  case 1:
    if (message == Arec)
      state3 = 2;
    else if (message != FLAG)
      state3 = 0;
    break;

  case 2:
    if (message == Crej0 && s == 0)
    {
      state3 = 3;
    }
    else if (message == Crej1 && s == 1)
    {
      state3 = 3;
    }
    else if (message == FLAG)
      state3 = 1;
    else
      state3 = 0;
    break;
  case 3:
    if ((message == (Arec ^ Crej0) && s == 0) || (message == (Arec ^ Crej1) && s == 1))
      state3 = 4;
    else if (message == FLAG)
      state3 = 1;
    else
      state3 = 0;
    break;
  case 4:
    if (message == FLAG)
      state3 = 5;
    else
      state3 = 0;

    break;
  default:
    state3 = 0;
  }
}
