
/*
 *  player.ino - Simple sound player example application
 *  Copyright 2018 Sony Semiconductor Solutions Corporation
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <SDHCI.h>

#include <Audio.h>
#include <fcntl.h>

SDClass theSD;

AudioClass *theAudio;

int fd;
File myFile;


void setup()
{
  // start audio system
  theAudio = AudioClass::getInstance();

  theAudio->begin();

  puts("initialization Audio Library");

  theAudio->setPlayerMode(AS_OUT_SP);

  theAudio->initPlayer(AudioClass::Player0, AS_CODECTYPE_MP3, "/mnt/sd0/BIN", AS_SAMPLINGRATE_AUTO,AS_CHANNEL_STEREO);

  myFile = theSD.open("DoorBell.mp3");
  printf("Open! %d\n",myFile);

  int err = theAudio->writeFrames(AudioClass::Player0, myFile);

  if (err != AUDIOLIB_ECODE_OK)
    {
      printf("File Read Error! =%d\n",err);
    }

  puts("Play!");

  theAudio->setVolume(10, 0, 0);
  theAudio->startPlayer(AudioClass::Player0);
}

void loop()
{

  // put your main code here, to run repeatedly:

  puts("loop!!");

  // for examle. Chack Bottom...

  int err = theAudio->writeFrames(AudioClass::Player0, myFile);

  if (err == AUDIOLIB_ECODE_FILEEND)
    {
      printf("File End! =%d\n",err);
      sleep(1);
      theAudio->stopPlayer(AudioClass::Player0);
      while(1);
    }

  usleep(40000);
}
