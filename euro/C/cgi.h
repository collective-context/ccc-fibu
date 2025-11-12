/*
 * This file is part of the LIBCGI library
 *
 */

/*
 * Copyright (c) 1995, Enterprise Integration Technologies
 * use, modification, and distribution rights restricted, see the LICENSE.txt
 * file for details
 */


#ifndef CGI_H
#define CGI_H

#define	DLLExport	__declspec( dllexport )

#define strcasecmp stricmp
#define strncasecmp strnicmp

#define MCODE_GET 1
#define MCODE_POST 2
#define MCODE_PUT 3
#define MCODE_HEAD 4

typedef struct
{
  char *server_software;
  char *server_name;
  char *gateway_interface;
  char *server_protocol;
  char *server_port;
  char *request_method;
  char *http_accept;
  char *path_info;
  char *path_translated;
  char *script_name;
  char *query_string;
  char *remote_host;
  char *remote_addr;
  char *auth_type;
  char *remote_user;
  char *remote_ident;
  char *content_type;
  int content_length;
} cgi_info;

typedef struct festruct
{
  char *name;
  char *val;
  struct festruct *next;
} form_entry;




/* Prototypes */
/* ---------- */

                        char *parmval(form_entry *, char *);
/*
extern	DLLExport	char *parmval(form_entry *, char *);
extern	DLLExport	form_entry *get_form_entries(cgi_info *);
extern	DLLExport	void print_input_blank(char *, unsigned, unsigned, char *);
extern	DLLExport	void print_sel_list(char *, char **, char *);
extern	DLLExport	void print_submit(char *);
extern	DLLExport	void print_doc_begin(char *);
extern	DLLExport	void print_doc_end(char *);
extern	DLLExport	char *trim(char *);
extern	DLLExport	char *sanitize(char *, char *);
extern	DLLExport	char *strmaxcpy(char *, char *, int);
extern	DLLExport	int get_cgi_info(cgi_info *ci);
extern	DLLExport	int syn_mimeheader(char *, char *);
extern	DLLExport	int print_mimeheader(char *);
extern	DLLExport	int syn_base_url(char *, cgi_info *);
extern	DLLExport	int print_base_url(cgi_info *);
extern	DLLExport	int mcode(cgi_info *);
extern	DLLExport	void free_form_entries(form_entry *);
extern	DLLExport	void print_doc_begin( char *title ) ;
extern	DLLExport	void print_doc_end(char *text);
*/

#endif
