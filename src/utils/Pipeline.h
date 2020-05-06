#ifndef _PIPELINE_H_
#define _PIPELINE_H_

//! Run a binary and feed it a given standard input
bool execWithStdin(const char* command, char* const args[], char* const env[], const char* stdin);

#endif