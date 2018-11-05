#include "data.h"
#include "stateMachine.h"

void changestate2Read(unsigned char message, unsigned char bcc);
void changestate2Write(unsigned char message);
void changestate2WriteREJ(unsigned char message);

int s = 0;
int state2 = 0;
int state3 = 0;

int count2 = 1;
int data_alarm = 0;
void touch2()
{
  data_alarm = 1;
  printf("Timeout number %d in Data message response. Resending data...\n", count2);
  count2++;
  if (count2 >= 4)
  {
    printf("Couldn't send data. Exiting...\n");
    exit(1);
  }
}

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
  printf("Bcc1:%x\n",bbc1);

  for (int i = 0; i < length; i++)
  {
    writeStuff(data[i], fd);
  }

  writeStuff(bcc2, fd);
  printf("Bcc2:%x\n",bbc2);
  send = FLAG;
  writeByte(fd, send);
}

int llwrite(int fd, unsigned char *data, int length)
{
  int res;
  unsigned char receive;
  data_alarm = 0;
  state2 = 0;
  state3 = 0;
  sendDataAux(data, length, fd);
  signal(SIGALRM, touch2);
  printf("Data sent. Waiting for the response...\n");
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
      printf("REJ message received ...\n");
      printf("Resending data...\n");
      sendDataAux(data, length, fd);
      printf("Data resent.\n");
      state2 = 0;
      state3 = 0;
      count2 = 1;
      alarm(WAITTIME);
      data_alarm = 0;
    }
  }
  alarm(0);
  printf("Response received.\n");
  count2 = 1;
  if (s == 0)
    s = 1;
  else
    s = 0;
  return length;
}

int llRead(int fd, unsigned char *guardar2)
{
  unsigned int bcc = 0, i = 0;
  unsigned char receive;
  unsigned char guardar[10 * PackageSize];
  state2 = 0;
  int res;
  int passei = 0;
  int k = 0;
  printf("Reading data...\n");
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
      if (passei != 1)
      {
        passei = 0;
        continue;
      }
      passei = 0;
      receive = 0;
      while (receive != FLAG && i <= MaximumRead)
      {
        res = read(fd, &receive, 1);
        if (res == 0)
          continue;
        guardar[i] = receive;
        i++;
      }
      if (i < 4)
      {
        printf("Error Reading...\n");
        state2 = 0;
        printf("Sending REJ message to Sender...\n");
        if (s == 0)
          sendMessage("REJ0", "R", fd);
        else
          sendMessage("REJ1", "R", fd);
        printf("REJ message sent. S: %d\n", s);
        continue;
      }

      if (i > MaximumRead)
        continue;
      i = i - 3;
      bcc = 0;           //TODO check this bcc1 e bcc2
      bcc ^= guardar[0]; //A
      bcc ^= guardar[1]; //C
      int j = 3, help = 0;
      if (unStuff(guardar[2], guardar[3]) != 0)
        j++;
      if (unStuff(guardar[i], guardar[i + 1]) != 0)
      {
        i--;
        help++;
      }
      changestate2Read(guardar[0], bcc);
      changestate2Read(guardar[1], bcc);
      printf("BCC1:%x\n",bcc);
      bcc = 0;
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
      printf("BCC2:%x\n",bcc);
      if (unStuff(guardar[2], guardar[3]) == 0)
        changestate2Read(guardar[2], bcc);
      else
        changestate2Read(unStuff(guardar[2], guardar[3]), bcc);
      if (unStuff(guardar[i + help], guardar[i + 1 + help]) == 0)
        changestate2Read(guardar[i + 1 + help], bcc);
      else
        changestate2Read(unStuff(guardar[i + help], guardar[i + 1 + help]), bcc);
      changestate2Read(guardar[i + 2 + help], bcc);
      if (state2 != 6)
      {
        printf("Sending REJ message to Sender...\n");
        if (s == 0)
          sendMessage("REJ0", "R", fd);
        else
          sendMessage("REJ1", "R", fd);
        printf("REJ message sent. S:%d\n", s);
        state2 = 0;
      }
    }
  }
  if (s)
    s = 0;
  else
    s = 1;
  printf("Data read. Sending RR message to Sender...\n");
  if (s == 0)
    sendMessage("RR0", "R", fd);
  else
    sendMessage("RR1", "R", fd);
  printf("RR message sent. \n");
  return k;
}

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
