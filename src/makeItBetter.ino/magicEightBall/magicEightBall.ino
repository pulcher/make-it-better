/*
  Making my own electronic 8 ball.
  Will randomly pick a mp3 file from a list of files on the SD card and play it.
*/

#include <SDHCI.h>
#include <Audio.h>
#include <MediaPlayer.h>
#include <OutputMixer.h>
#include <MemoryUtil.h>

#define SYSTEM_INIT_MP3 "init.mp3"
#define MAX_QUIPS 64

const int buttonPin = 2;      // Pin used to detect a button press
const int ledPin    = 13;     // Pin for the highside of the button LED

SDClass theSD;
AudioClass *theAudio;
File myDir;
err_t err;

int numQuips          = 0;
int currentQuip       = 0;
bool ErrEnd           = false;
bool audioInitialized = false;
String foundQuips[MAX_QUIPS];

// Variables used in interrupt service routines and other parts of the program should be volatile
// 2 is used to indicate there has not been an interrupt yet
// 1 is button released
// 0 is button pressed
volatile int buttonState = 2;

// should we play a quip?
volatile bool canPlayQuip = false;

/**
 * Audio attention callback
 *
 * When audio internal error occurc, this function will be called back.
 */
static void audio_attention_cb(const ErrorAttentionParam *atprm)
{
  puts("Attention!");

  if (atprm->error_code >= AS_ATTENTION_CODE_WARNING)
  {
    ErrEnd = true;
  }

  setupAudio();
}

void setup() {
  Serial.begin(115200);

  Serial.println("initializing storage...");
  initializeQuips();

  Serial.println("Reading available quips from the SD card...");
  myDir = theSD.open("/");
  getQuips(myDir, 0);
  myDir.close();

  Serial.print("Found ");
  Serial.print(numQuips);
  Serial.println(" quips...");
  for(int i = 0; i < numQuips; i++)
  {
    Serial.println(foundQuips[i]);
  }

  // setup the button and LED
  pinMode(ledPin, OUTPUT);      // will raise and lower this output to toggle the LED.
  pinMode(buttonPin, INPUT);    // will watch this pin to detect the button press.

  // attaching and interrupt to the pin
  attachInterrupt(buttonPin, pinIsr, CHANGE);

  setupAudio();

  playQuip(SYSTEM_INIT_MP3);
}

void loop() {

  if (canPlayQuip)
  {
    playQuip(foundQuips[currentQuip]);
    //playQuip(SYSTEM_INIT_MP3);
    currentQuip++;
    Serial.print("currentQuip: ");
    Serial.println(currentQuip);
  }

  if (currentQuip >= numQuips)
  {
    currentQuip = 0;
  }
  delay(200);
}

void pinIsr()
{
  buttonState = digitalRead(buttonPin);

  digitalWrite(ledPin, buttonState);

  if (buttonState == 1)
    canPlayQuip = true;

  delayMicroseconds(200);
}

void initializeQuips()
{
  for(int i = 0; i < 32; i++)
  {
    foundQuips[i] = "";   // lots of empty strings.
  }

  numQuips = 0;
}

void getQuips(File dir, int numTabs) {

  String endTest = ".mp3";

  while (true) {

    File entry =  dir.openNextFile();
    if (!entry || numQuips >= MAX_QUIPS) {
      // no more files
      break;
    }

    String entryName = entry.name();
    entryName.remove(0,1);

    if (!entry.isDirectory() && entryName.endsWith(endTest) && !entryName.equals("init.mp3"))
    {
      foundQuips[numQuips] = entryName;
      numQuips++;
    }

    entry.close();
  }
}

void playQuip(String fileName)
{
  canPlayQuip = false;

  Serial.print("Playing: ");
  Serial.println(fileName);
  
  /* Open file placed on SD card */
  File myFile = theSD.open(fileName);

  /* Verify file open */
  if (!myFile)
  {
    printf("File open error\n");
    exit(1);
  }
  
  printf("Open! %s\n", myFile.name());

  theAudio->setPlayerMode(AS_SETPLAYER_OUTPUTDEVICE_SPHP, AS_SP_DRV_MODE_LINEOUT);

  puts("player initialization");
  
  /*
   * Set main player to decode stereo mp3. Stream sample rate is set to "auto detect"
   * Search for MP3 decoder in "/mnt/sd0/BIN" directory
   */
  
  err = theAudio->initPlayer(AudioClass::Player0, AS_CODECTYPE_MP3, "/mnt/sd0/BIN", AS_SAMPLINGRATE_AUTO, AS_CHANNEL_MONO);
  
  /* Send first frames to be decoded */
  err = theAudio->writeFrames(AudioClass::Player0, myFile);

  printf("Error: %d\n", err);

  if ((err != AUDIOLIB_ECODE_OK) && (err != AUDIOLIB_ECODE_FILEEND))
  {
    printf("File Read Error! =%d\n",err);
    myFile.close();
    exit(1);
  }

  puts("Play!");

  theAudio->startPlayer(AudioClass::Player0);

  delay(100);

  puts("Stop!");
  sleep(1);

  theAudio->stopPlayer(AudioClass::Player0);

  puts("closing file");
  myFile.close();

  puts("returning to ready mode");
  theAudio->setReadyMode();
}

void setupAudio()
{
  puts("checking audio initialization");
 
  // make sure we are not calling this if there is nothing to call
  if(audioInitialized)
  {
    puts("shutting down the audio subsystem");
    //theAudio->end();
    sleep(1);
    audioInitialized = false;
  }

  // start audio system
  theAudio = AudioClass::getInstance();

  theAudio->begin(audio_attention_cb);

  puts("initialization Audio Library");

  /* Set clock mode to normal */
  theAudio->setRenderingClockMode(AS_CLKMODE_NORMAL);

  puts("setting player mode");

  /* Verify player initialize */
  if (err != AUDIOLIB_ECODE_OK)
  {
    printf("Player0 initialize error\n");
    exit(1);
  }

  /* Main volume set to -16.0 dB */
  theAudio->setVolume(60);

  audioInitialized = true;
}
