/***************************************************************************
 *   Copyright (C) 2006 to 2013 by Jonathan Duddington                     *
 *   email: jonsd@users.sourceforge.net                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write see:                           *
 *               <http://www.gnu.org/licenses/>.                           *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifndef NEED_GETOPT
#include <getopt.h>
#endif
#include <time.h>
#include <sys/stat.h>

#include "speak_lib.h"

#include <iostream>
#include <sstream>
#include <string>

// This version of the command-line speak program uses the
// libespeak.so.1  library



static const char *help_text =
"\nespeak [options] [\"<words>\"]\n\n"
"-f <text file>   Text file to speak\n"
"--stdin    Read text input from stdin instead of a file\n\n"
"If neither -f nor --stdin, then <words> are spoken, or if none then text\n"
"is spoken from stdin, each line separately.\n\n"
"-a <integer>\n"
"\t   Amplitude, 0 to 200, default is 100\n"
"-g <integer>\n"
"\t   Word gap. Pause between words, units of 10mS at the default speed\n"
"-k <integer>\n"
"\t   Indicate capital letters with: 1=sound, 2=the word \"capitals\",\n"
"\t   higher values indicate a pitch increase (try -k20).\n"
"-l <integer>\n"
"\t   Line length. If not zero (which is the default), consider\n"
"\t   lines less than this length as end-of-clause\n"
"-p <integer>\n"
"\t   Pitch adjustment, 0 to 99, default is 50\n"
"-s <integer>\n"
"\t   Speed in words per minute, 80 to 450, default is 175\n"
"-v <voice name>\n"
"\t   Use voice file of this name from espeak-data/voices\n"
"-w <wave file name>\n"
"\t   Write speech to this WAV file, rather than speaking it directly\n"
"-b\t   Input text encoding, 1=UTF8, 2=8 bit, 4=16 bit \n"
"-m\t   Interpret SSML markup, and ignore other < > tags\n"
"-q\t   Quiet, don't produce any speech (may be useful with -x)\n"
"-x\t   Write phoneme mnemonics to stdout\n"
"-X\t   Write phonemes mnemonics and translation trace to stdout\n"
"-z\t   No final sentence pause at the end of the text\n"
"--compile=<voice name>\n"
"\t   Compile pronunciation rules and dictionary from the current\n"
"\t   directory. <voice name> specifies the language\n"
"--ipa      Write phonemes to stdout using International Phonetic Alphabet\n"
"\t         --ipa=1 Use ties, --ipa=2 Use ZWJ, --ipa=3 Separate with _\n" 
"--path=\"<path>\"\n"
"\t   Specifies the directory containing the espeak-data directory\n"
"--pho      Write mbrola phoneme data (.pho) to stdout or to the file in --phonout\n"
"--phonout=\"<filename>\"\n"
"\t   Write phoneme output from -x -X --ipa and --pho to this file\n"
"--punct=\"<characters>\"\n"
"\t   Speak the names of punctuation characters during speaking.  If\n"
"\t   =<characters> is omitted, all punctuation is spoken.\n"
"--split=\"<minutes>\"\n"
"\t   Starts a new WAV file every <minutes>.  Used with -w\n"
"--stdout   Write speech output to stdout\n"
"--version  Shows version number and date, and location of espeak-data\n"
"--voices=<language>\n"
"\t   List the available voices for the specified language.\n"
"\t   If <language> is omitted, then list all voices.\n";




int samplerate;
int quiet = 0;
unsigned int samples_total = 0;
unsigned int samples_split = 0;
unsigned int samples_split_seconds = 0;
unsigned int wavefile_count = 0;

FILE *f_wavfile = NULL;
char filetype[5];
char wavefile[200];


int GetFileLength(const char *filename)
{//====================================
	struct stat statbuf;

	if(stat(filename,&statbuf) != 0)
		return(0);

	if((statbuf.st_mode & S_IFMT) == S_IFDIR)
		return(-2);  // a directory

	return(statbuf.st_size);
}  // end of GetFileLength


void strncpy0(char *dest, const char *source, int size)
{//====================================================
	if(source!=NULL)
	{
		strncpy(dest,source,size);
		dest[size-1] = 0;
	}
}


void DisplayVoices(FILE *f_out, char *language)
{//============================================
	int ix;
	const char *p;
	int len;
	int count;
	int c;
	int j;
	const espeak_VOICE *v;
	const char *lang_name;
	char age_buf[12];
	char buf[80];
	const espeak_VOICE **voices;
	espeak_VOICE voice_select;

	static char genders[4] = {'-','M','F','-'};

	if((language != NULL) && (language[0] != 0))
	{
		// display only voices for the specified language, in order of priority
		voice_select.languages = language;
		voice_select.age = 0;
		voice_select.gender = 0;
		voice_select.name = NULL;
		voices = espeak_ListVoices(&voice_select);
	}
	else
	{
		voices = espeak_ListVoices(NULL);
	}

	fprintf(f_out,"Pty Language Age/Gender VoiceName          File          Other Languages\n");

	for(ix=0; (v = voices[ix]) != NULL; ix++)
	{
		count = 0;
		p = v->languages;
		while(*p != 0)
		{
			len = strlen(p+1);
			lang_name = p+1;

			if(v->age == 0)
				strcpy(age_buf,"   ");
			else
				sprintf(age_buf,"%3d",v->age);

			if(count==0)
			{
				for(j=0; j < sizeof(buf); j++)
				{
					// replace spaces in the name
					if((c = v->name[j]) == ' ')
						c = '_';
					if((buf[j] = c) == 0)
						break;
				}
				fprintf(f_out,"%2d  %-12s%s%c  %-20s %-13s ",
               p[0],lang_name,age_buf,genders[v->gender],buf,v->identifier);
			}
			else
			{
				fprintf(f_out,"(%s %d)",lang_name,p[0]);
			}
			count++;
			p += len+2;
		}
		fputc('\n',f_out);
	}
}   //  end of DisplayVoices




static void Write4Bytes(FILE *f, int value)
{//=================================
// Write 4 bytes to a file, least significant first
	int ix;

	for(ix=0; ix<4; ix++)
	{
		fputc(value & 0xff,f);
		value = value >> 8;
	}
}



int OpenWavFile(char *path, int rate)
//===================================
{
	static unsigned char wave_hdr[44] = {
		'R','I','F','F',0x24,0xf0,0xff,0x7f,'W','A','V','E','f','m','t',' ',
		0x10,0,0,0,1,0,1,0,  9,0x3d,0,0,0x12,0x7a,0,0,
		2,0,0x10,0,'d','a','t','a',  0x00,0xf0,0xff,0x7f};

	if(path == NULL)
		return(2);

	while(isspace(*path)) path++;

	f_wavfile = NULL;
	if(path[0] != 0)
	{
		if(strcmp(path,"stdout")==0)
			f_wavfile = stdout;
		else
			f_wavfile = fopen(path,"wb");
	}
	
	if(f_wavfile == NULL)
	{
		fprintf(stderr,"Can't write to: '%s'\n",path);
		return(1);
	}


	fwrite(wave_hdr,1,24,f_wavfile);
	Write4Bytes(f_wavfile,rate);
	Write4Bytes(f_wavfile,rate * 2);
	fwrite(&wave_hdr[32],1,12,f_wavfile);
	return(0);
}   //  end of OpenWavFile



static void CloseWavFile()
//========================
{
	unsigned int pos;

	if((f_wavfile==NULL) || (f_wavfile == stdout))
		return;

	fflush(f_wavfile);
	pos = ftell(f_wavfile);

	fseek(f_wavfile,4,SEEK_SET);
	Write4Bytes(f_wavfile,pos - 8);

	fseek(f_wavfile,40,SEEK_SET);
	Write4Bytes(f_wavfile,pos - 44);

	fclose(f_wavfile);
	f_wavfile = NULL;

} // end of CloseWavFile


static int SynthCallback(short *wav, int numsamples, espeak_EVENT *events)
{//========================================================================
	char fname[210];

	if(quiet) return(0);  // -q quiet mode

	if(wav == NULL)
	{
		CloseWavFile();
		return(0);
	}

	while(events->type != 0)
	{
		if(events->type == espeakEVENT_SAMPLERATE)
		{
			samplerate = events->id.number;
			samples_split = samples_split_seconds * samplerate;
		}
		else
		if(events->type == espeakEVENT_SENTENCE)
		{
			// start a new WAV file when the limit is reached, at this sentence boundary
			if((samples_split > 0) && (samples_total > samples_split))
			{
				CloseWavFile();
				samples_total = 0;
				wavefile_count++;
			}
		}
		events++;
	}

	if(f_wavfile == NULL)
	{
		if(samples_split > 0)
		{
			sprintf(fname,"%s_%.2d%s",wavefile,wavefile_count+1,filetype);
			if(OpenWavFile(fname, samplerate) != 0)
				return(1);
		}
		else
		{
			if(OpenWavFile(wavefile, samplerate) != 0)
				return(1);
		}
	}

	if(numsamples > 0)
	{
		samples_total += numsamples;
		fwrite(wav,numsamples*2,1,f_wavfile);
	}
	return(0);
}


static void PrintVersion()
{//=======================
	const char *version;
	const char *path_data;
	espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, NULL, espeakINITIALIZE_DONT_EXIT);
	version = espeak_Info(&path_data);
	printf("eSpeak text-to-speech: %s  Data at: %s\n", version, path_data);
}



#ifdef NEED_GETOPT
	struct option {
		char *name;
		int has_arg;
		int *flag;
		int val;
	};
	int optind;
	static int optional_argument;
	static const char *arg_opts = "abfgklpsvw";  // which options have arguments
	static char *opt_string="";
#define no_argument 0
#define required_argument 1
#define optional_argument 2
#endif


int main (int argc, char **argv)
//==============================
{
	static struct option long_options[] =
		{
		/* These options set a flag. */
//		{"verbose", no_argument,       &verbose_flag, 1},
//		{"brief",   no_argument,       &verbose_flag, 0},

		/* These options don't set a flag.
			We distinguish them by their indices. */
		{"help",    no_argument,       0, 'h'},
		{"stdin",   no_argument,       0, 0x100},
		{"compile-debug", optional_argument, 0, 0x101},
		{"compile", optional_argument, 0, 0x102},
		{"punct",   optional_argument, 0, 0x103},
		{"voices",  optional_argument, 0, 0x104},
		{"stdout",  no_argument,       0, 0x105},
		{"split",   optional_argument, 0, 0x106},
		{"path",    required_argument, 0, 0x107},
		{"phonout", required_argument, 0, 0x108},
		{"pho",     no_argument,       0, 0x109},
		{"ipa",     optional_argument, 0, 0x10a},
		{"version", no_argument,       0, 0x10b},
		{0, 0, 0, 0}
		};

	static const char* err_load = "Failed to read ";


	FILE *f_text=NULL;
	char *p_text=NULL;
	FILE *f_phonemes_out = stdout;
	char *data_path = NULL;   // use default path for espeak-data

	int option_index = 0;
	int c;
	int ix;
	char *optarg2;
	int value;
	int flag_stdin = 0;
	int flag_compile = 0;
	int filesize = 0;
	int synth_flags = espeakCHARS_AUTO | espeakPHONEMES | espeakENDPAUSE;

	int volume = -1;
	int speed = -1;
	int pitch = -1;
	int wordgap = -1;
	int option_capitals = -1;
	int option_punctuation = -1;
	int option_phonemes = 0;
	int option_mbrola_phonemes = 0;
	int option_linelength = 0;
	int option_waveout = 0;

	espeak_VOICE voice_select;
	char filename[200];
	char voicename[40];
#define N_PUNCTLIST  100
	wchar_t option_punctlist[N_PUNCTLIST];

	voicename[0] = 0;
	wavefile[0] = 0;
	filename[0] = 0;
	option_punctlist[0] = 0;

    std::string test = "/usr/share";
    data_path = (char *)test.c_str();


    // play the sound output
    samplerate = espeak_Initialize(AUDIO_OUTPUT_PLAYBACK,0,data_path,0);

    strcpy(voicename, "de");
    if(espeak_SetVoiceByName(voicename) != EE_OK)
    {
        memset(&voice_select,0,sizeof(voice_select));
        voice_select.languages = voicename;
        if(espeak_SetVoiceByProperties(&voice_select) != EE_OK)
        {
            fprintf(stderr,"%svoice '%s'\n",err_load,voicename);
            exit(2);
        }
    }

    espeak_SetParameter(espeakWORDGAP,20,0);

	espeak_SetPhonemeTrace(option_phonemes | option_mbrola_phonemes,f_phonemes_out);


    test = "liebe oma alles gute zum geburtstag";
    p_text = (char *)test.c_str();

    int size;
    size = strlen(p_text);
    espeak_Synth(p_text,size+1,0,POS_CHARACTER,0,synth_flags,NULL,NULL);


	if(espeak_Synchronize() != EE_OK)
	{
		fprintf(stderr, "espeak_Synchronize() failed, maybe error when opening output device\n");
		exit(4);
	}

	if(f_phonemes_out != stdout)
		fclose(f_phonemes_out);  // needed for WinCE
	return(0);
}
