/* $Id: upnphttp.c,v 1.2 2008/03/02 10:16:43 jasonwang Exp $ */
/* Project :  miniupnp
 * Website :  http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * Author :   Thomas Bernard
 * Copyright (c) 2005 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file included in this distribution.
 * */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <ctype.h>
#include <syslog.h>
#include "upnphttp.h"
#include "upnpdescgen.h"
#include "miniupnpdpath.h"
#include "upnpsoap.h"

#ifdef CONFIG_RANDOM_URL
#include "upnpglobalvars.h"
#endif /* CONFIG_RANDOM_URL */

#if defined(ENABLE_EVENTS)
#include "upnpevents.h"
#endif
struct upnphttp * New_upnphttp(int s)
{
	struct upnphttp * ret;
	if(s<0)
		return NULL;
	ret = (struct upnphttp *)malloc(sizeof(struct upnphttp));
	if(ret == NULL)
		return NULL;
	memset(ret, 0, sizeof(struct upnphttp));
	ret->socket = s;
	return ret;
}

void CloseSocket_upnphttp(struct upnphttp * h)
{
	close(h->socket);
	h->socket = -1;
	h->state = 100;
}

void Delete_upnphttp(struct upnphttp * h)
{
	if(h)
	{
		if(h->socket >= 0)
			close(h->socket);
		if(h->req_buf)
			free(h->req_buf);
		if(h->res_buf)
			free(h->res_buf);
		free(h);
	}
}

/* parse HttpHeaders of the REQUEST */
static void ParseHttpHeaders(struct upnphttp * h)
{
	char * line;
	char * colon;
	char * p;
	int n;
	line = h->req_buf;
	/* TODO : check if req_buf, contentoff are ok */
	while(line < (h->req_buf + h->req_contentoff))
	{
		colon = strchr(line, ':');
		if(colon)
		{
			if(strncasecmp(line, "Content-Length", 14)==0)
			{
				p = colon;
				while(*p < '0' || *p > '9')
					p++;
				h->req_contentlen = atoi(p);
				/*printf("*** Content-Lenght = %d ***\n", h->req_contentlen);
				printf("    readbufflen=%d contentoff = %d\n",
					h->req_buflen, h->req_contentoff);*/
			}
			else if(strncasecmp(line, "Host", 4)==0)
			{
				p = colon;
				n = 0;
				while(*p == ':' || *p == ' ' || *p == '\t')
					p++;
				while(p[n]>' ')
					n++;
				h->req_HostOff = p - h->req_buf;
				h->req_HostLen = n;
			}
			else if(strncasecmp(line, "SOAPAction", 10)==0)
			{
				p = colon;
				n = 0;
				while(*p == ':' || *p == ' ' || *p == '\t')
					p++;
				while(p[n]>=' ')
				{
					n++;
				}
				if((p[0] == '"' && p[n-1] == '"')
				  || (p[0] == '\'' && p[n-1] == '\''))
				{
					p++; n -= 2;
				}
				h->req_soapAction = p;
				h->req_soapActionLen = n;
			}
#ifdef ENABLE_EVENTS
			else if(strncasecmp(line, "Callback", 8)==0)
			{
				p = colon;
				while(*p != '<' && *p != '\r' )
					p++;
					n = 0;
				while(p[n] != '>' && p[n] != '\r' )
					n++;
					h->req_Callback = p + 1;
					h->req_CallbackLen = MAX(0, n - 1);
			}
			else if(strncasecmp(line, "SID", 3)==0)
			{
				p = colon + 1;
				while(isspace(*p)){
					if(*p == '\r'){
						break;
					}
					p++;
				}
					n = 0;
				while(!isspace(p[n])){
					n++;
				}
				if(n > 0)
				h->req_SID = p;
				else
					h->req_SID = NULL;
				h->req_SIDLen = n;
			}
#ifdef CONFIG_CERTIFICATION
			else if(strncasecmp(line, "NT", 2)==0)
			{
				p = colon + 1;
				while(isspace(*p))
					p++;
				if(strncasecmp(p, "upnp:event", 10)==0)
					h->nt = 1;
				else
					h->nt = 0;
			}
#endif
			/* Timeout: Seconds-nnnn */
			/* TIMEOUT
			Recommended. Requested duration until subscription expires,
			either number of seconds or infinite. Recommendation
			by a UPnP Forum working committee. Defined by UPnP vendor.
			 Consists of the keyword "Second-" followed (without an
			intervening space) by either an integer or the keyword "infinite". */
			else if(strncasecmp(line, "Timeout", 7)==0)
			{
				p = colon + 1;
				while(isspace(*p))
					p++;
				if(strncasecmp(p, "Second-", 7)==0) {
					if(strncasecmp(p+7, "infinite", 8)==0){
						h->req_Timeout = 0xFFFFFFFF;
					}else
					h->req_Timeout = atoi(p+7);
					}
			}
#ifdef CONFIG_CERTIFICATION
			else if(strncasecmp(line, "Accept-Language", 15)==0)
			{
				h->respflags |= FLAG_LANGUAGE;
			}
#endif
#endif
		}
		while(!(line[0] == '\r' && line[1] == '\n'))
			line++;
		line += 2;
	}
}

/* very minimalistic 404 error message */
static void Send404(struct upnphttp * h)
{
	static const char error404[] = "HTTP/1.1 404 Not found\r\n"
		"Connection: close\r\n"
		"Content-type: text/html\r\n"
		"\r\n"
		"<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD>"
		"<BODY><H1>Not Found</H1>The requested URL was not found"
		" on this server.</BODY></HTML>\r\n";
	int n;
// david	
//	n = send(h->socket, error404, sizeof(error404) - 1, 0);
	n = send(h->socket, error404, sizeof(error404) - 1, MSG_DONTROUTE | MSG_NOSIGNAL);
	if(n < 0)
	{
		syslog(LOG_ERR, "Send404: send: %m");
	}
	CloseSocket_upnphttp(h);
}

/* very minimalistic 501 error message */
static void Send501(struct upnphttp * h)
{
	static const char error501[] = "HTTP/1.1 501 Not Implemented\r\n"
		"Connection: close\r\n"
		"Content-type: text/html\r\n"
		"\r\n"
		"<HTML><HEAD><TITLE>501 Not Implemented</TITLE></HEAD>"
		"<BODY><H1>Not Implemented</H1>The HTTP Method "
		"is not implemented by this server.</BODY></HTML>\r\n";
	int n;
//david	
//	n = send(h->socket, error501, sizeof(error501) - 1, 0);
	n = send(h->socket, error501, sizeof(error501) - 1, MSG_DONTROUTE | MSG_NOSIGNAL);
	if(n < 0)
	{
		syslog(LOG_ERR, "Send501: send: %m");
	}
	CloseSocket_upnphttp(h);
}

static const char * findendheaders(const char * s, int len)
{
	while(len-->0)
	{
		if(s[0]=='\r' && s[1]=='\n' && s[2]=='\r' && s[3]=='\n')
			return s;
		s++;
	}
	return NULL;
}

static void sendDummyDesc(struct upnphttp * h)
{
	static const char xml_desc[] = "<?xml version=\"1.0\"?>\n"
		"<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">"
		" <specVersion>"
		"    <major>1</major>"
		"    <minor>0</minor>"
		"  </specVersion>"
		"  <actionList />"
		"  <serviceStateTable />"
		"</scpd>";
	BuildResp_upnphttp(h, xml_desc, sizeof(xml_desc)-1);
	SendResp_upnphttp(h);
	CloseSocket_upnphttp(h);
}

/* Sends the description generated by the parameter */
static void sendXMLdesc(struct upnphttp * h, char * (f)(int *))
{
	char * desc;
	int len;
#ifdef ENABLE_IPV6
		if(f == genRootDesc)
			if(h->ipv6)
				desc = genRootDesc6(&len);
			else
				desc = genRootDesc(&len);
		else
			desc = f(&len);
#else
	desc = f(&len);
#endif	
	if(!desc)
	{
		syslog(LOG_ERR, "XML description generation failed");
		return;
	}
	BuildResp_upnphttp(h, desc, len);
	SendResp_upnphttp(h);
	CloseSocket_upnphttp(h);
	free(desc);
}
#ifdef ENABLE_EVENTS
static void ProcessHTTPSubscribe_upnphttp(struct upnphttp * h, const char * path)
{
	const char * sid=NULL;
	//syslog(LOG_DEBUG, "ProcessHTTPSubscribe %s", path);
	//syslog(LOG_DEBUG, "Callback '%.*s' Timeout=%d",h->req_CallbackLen, h->req_Callback, h->req_Timeout);
	//syslog(LOG_DEBUG, "SID '%.*s'", h->req_SIDLen, h->req_SID);
#ifdef CONFIG_CERTIFICATION
	if((h->req_Callback || h->nt)&&h->req_SID) {
		
		BuildResp2_upnphttp(h,400, "Bad Request",  0, 0);
		SendResp_upnphttp(h);
		CloseSocket_upnphttp(h);
	}
	else if((!h->req_Callback || !h->nt)&&!h->req_SID){
#else
	if(!h->req_Callback && !h->req_SID) {
#endif
		/* Missing or invalid CALLBACK : 412 Precondition Failed.
		 * If CALLBACK header is missing or does not contain a valid HTTP URL,
		 * the publisher must respond with HTTP error 412 Precondition Failed*/
		BuildResp2_upnphttp(h, 412, "Precondition Failed", 0, 0);
		SendResp_upnphttp(h);
		CloseSocket_upnphttp(h);
	}
	else {
		/* - add to the subscriber list
		 * - respond HTTP/x.x 200 OK 
		 * - Send the initial event message */
		/* Server:, SID:; Timeout: Second-(xx|infinite) */
		if(h->req_Callback) {
			sid = upnpevents_addSubscriber(path, h->req_Callback,
			                               h->req_CallbackLen, h->req_Timeout);
			h->respflags = FLAG_TIMEOUT;
			if(sid) {
				//syslog(LOG_DEBUG, "generated sid=%s", sid);
				h->respflags |= FLAG_SID;
				h->req_SID = sid;
				h->req_SIDLen = strlen(sid);
				BuildResp_upnphttp(h, 0, 0);
			}else{
				BuildResp2_upnphttp(h, 412, "Precondition Failed", 0, 0);
			}
			
		} else {
			/* subscription renew */
			/* Invalid SID
				412 Precondition Failed. If a SID does not correspond to a known,
				un-expired subscription, the publisher must respond
				with HTTP error 412 Precondition Failed. */
			if(renewSubscription(h->req_SID, h->req_SIDLen, h->req_Timeout) < 0) {
				BuildResp2_upnphttp(h, 412, "Precondition Failed", 0, 0);
			} else {
				BuildResp_upnphttp(h, 0, 0);
			}
		}
		SendResp_upnphttp(h);
		CloseSocket_upnphttp(h);
	}
}

static void ProcessHTTPUnSubscribe_upnphttp(struct upnphttp * h, const char * path)
{
	//syslog(LOG_DEBUG, "ProcessHTTPUnSubscribe %s", path);
	//syslog(LOG_DEBUG, "SID '%.*s'", h->req_SIDLen, h->req_SID);
	//syslog(LOG_DEBUG, "SID '%.*s'", h->req_SIDLen, h->req_SID);
	/* Remove from the list */
#ifdef CONFIG_CERTIFICATION
	if((h->req_Callback || h->nt)&&h->req_SID) {		
		BuildResp2_upnphttp(h,400, "Bad Request",  0, 0);
		SendResp_upnphttp(h);
		CloseSocket_upnphttp(h);
	}
	else if(!h->req_SID && !h->req_SIDLen) {
#else
	if(!h->req_SID && !h->req_SIDLen) {
#endif
		/* Missing or invalid SID : 412 Precondition Failed.*/
		BuildResp2_upnphttp(h, 412, "Precondition Failed", 0, 0);
		SendResp_upnphttp(h);
		CloseSocket_upnphttp(h);
	}else{
		if(upnpevents_removeSubscriber(h->req_SID, h->req_SIDLen) < 0) {
			BuildResp2_upnphttp(h, 412, "Precondition Failed", 0, 0);
		} else {
			BuildResp_upnphttp(h, 0, 0);
		}
	SendResp_upnphttp(h);
	CloseSocket_upnphttp(h);
	}
}
#endif



/* ProcessHTTPPOST_upnphttp()
 * executes the SOAP query if it is possible */
static void ProcessHTTPPOST_upnphttp(struct upnphttp * h)
{
	if((h->req_buflen - h->req_contentoff) >= h->req_contentlen)
	{
		if(h->req_soapAction)
		{
			/* we can process the request */
//			syslog(LOG_INFO, "SOAPAction: %.*s",
//		    	   h->req_soapActionLen, h->req_soapAction);
			ExecuteSoapAction(h, 
				h->req_soapAction,
				h->req_soapActionLen);
		}
		else
		{
			static const char err400str[] =
				"<html><body>Bad request</body></html>";
			syslog(LOG_INFO, "No SOAPAction in HTTP headers");
			BuildResp2_upnphttp(h, 400, "Bad Request",
			                    err400str, sizeof(err400str) - 1);
			SendResp_upnphttp(h);
			CloseSocket_upnphttp(h);
		}
	}
	else
	{
		/* waiting for remaining data */
		h->state = 1;
	}
}

/* Parse and process Http Query 
 * called once all the HTTP headers have been received. */
static void ProcessHttpQuery_upnphttp(struct upnphttp * h)
{
	char HttpCommand[16];
	char HttpUrl[128];
	char * HttpVer;
	char * p;
	int i;
	p = h->req_buf;
	if(!p)
		return;
	for(i = 0; i<15 && *p != ' ' && *p != '\r'; i++)
		HttpCommand[i] = *(p++);
	HttpCommand[i] = '\0';
	while(*p==' ')
		p++;
	for(i = 0; i<127 && *p != ' ' && *p != '\r'; i++)
		HttpUrl[i] = *(p++);
	HttpUrl[i] = '\0';
	while(*p==' ')
		p++;
	HttpVer = h->HttpVer;
	for(i = 0; i<15 && *p != '\r'; i++)
		HttpVer[i] = *(p++);
	HttpVer[i] = '\0';
//	syslog(LOG_INFO, "HTTP REQUEST : %s %s (%s)",
//	       HttpCommand, HttpUrl, HttpVer);
	ParseHttpHeaders(h);
	if(h->req_HostOff > 0 && h->req_HostLen > 0) {
		syslog(LOG_DEBUG, "Host: %.*s", h->req_HostLen, h->req_buf + h->req_HostOff);			 
		p = h->req_buf + h->req_HostOff;
	
		if(*p == '[') {
			/* IPv6 */
			p++;
			while(p < h->req_buf + h->req_HostOff + h->req_HostLen) {
				if(*p == ']') break;
				/* TODO check *p in [0-9a-f:.] */
				p++;
			}
			if(*p != ']') {
				syslog(LOG_NOTICE, "DNS rebinding attack suspected (Host: %.*s)", h->req_HostLen, h->req_buf + h->req_HostOff);
				Send404(h);
				return;
			}
			p++;
			/* TODO : Check port */
		} else {
			for(i = 0; i < h->req_HostLen; i++) {
				if(*p != ':' && *p != '.' && (*p > '9' || *p < '0')) {
					syslog(LOG_NOTICE, "DNS rebinding attack suspected (Host: %.*s)", h->req_HostLen, h->req_buf + h->req_HostOff);
					Send404(h);
					return;
				}
				p++;
			}
		}
	}
#ifdef CONFIG_RANDOM_URL
	/* first check if the URL begins with the randomized string */
	if(HttpUrl[0] != '/' || memcmp(HttpUrl+1, random_url, strlen(random_url)) != 0)
	{
		Send404(h);
		return;
	}
	/* remove "random" from the start of the URL */
	p = HttpUrl + strlen(random_url) + 1;
	memmove(HttpUrl, p, strlen(p) + 1);
#endif
	if(strcmp("POST", HttpCommand) == 0)
	{
		h->req_command = EPost;
		ProcessHTTPPOST_upnphttp(h);
	}
	else if(strcmp("GET", HttpCommand) == 0)
	{
		h->req_command = EGet;
		if(strcmp(ROOTDESC_PATH, HttpUrl) == 0)
		{
			sendXMLdesc(h, genRootDesc);
		}
		else if(strcmp(WANIPC_PATH, HttpUrl) == 0)
		{
			sendXMLdesc(h, genWANIPCn);
		}
		else if(strcmp(WANCFG_PATH, HttpUrl) == 0)
		{
			sendXMLdesc(h, genWANCfg);
		}
#ifdef ENABLE_6FC_SERVICE
		else if(strcmp(WANIP6FC_PATH, HttpUrl) == 0)
		{
			sendXMLdesc(h, gen6FC);
		}
#endif
		else if(strcmp(DUMMY_PATH, HttpUrl) == 0)
		{
			sendDummyDesc(h);
		}
		else
		{
			syslog(LOG_NOTICE, "%s not found, responding ERROR 404", HttpUrl);
			Send404(h);
		}
	}
#ifdef ENABLE_EVENTS
	else if(strcmp("SUBSCRIBE", HttpCommand) == 0)
	{
		if(strcmp("/dummy", HttpUrl) == 0){
		Send501(h);
		}else{
		h->req_command = ESubscribe;
		ProcessHTTPSubscribe_upnphttp(h, HttpUrl);
	}
	}
	else if(strcmp("UNSUBSCRIBE", HttpCommand) == 0)
	{
		if(strcmp("/dummy", HttpUrl) == 0){
			Send501(h);
		}else{
		h->req_command = EUnSubscribe;
		ProcessHTTPUnSubscribe_upnphttp(h, HttpUrl);
	}
	}
#else
	else if(strcmp("SUBSCRIBE", HttpCommand) == 0)
	{
		//syslog(LOG_NOTICE, "SUBSCRIBE not implemented yet");
		Send501(h);
	}
#endif
	else
	{
		syslog(LOG_NOTICE, "Unsupported HTTP Command %s", HttpCommand);
		Send501(h);
	}
}


void Process_upnphttp(struct upnphttp * h)
{
	char buf[2048];
	int n;
	if(!h)
		return;
	switch(h->state)
	{
	case 0:
		n = recv(h->socket, buf, 2048, MSG_DONTWAIT);
		if(n<0)
		{
			syslog(LOG_ERR, "recv (state0): %m");
			h->state = 100;
		}
		else if(n==0)
		{
			syslog(LOG_WARNING, "connection closed inexpectedly");
			h->state = 100;
		}
		else
		{
			const char * endheaders;
			/*printf("== PACKET RECEIVED (%d bytes) ==\n", n);
			fwrite(buf, 1, n, stdout);	// debug
			printf("== END OF PACKET RECEIVED ==\n");*/
			/* if 1st arg of realloc() is null,
			 * realloc behaves the same as malloc() */
			h->req_buf = (char *)realloc(h->req_buf, n + h->req_buflen + 1);
			if(!h->req_buf){
				h->state = 100;
				break;
			}
			memcpy(h->req_buf + h->req_buflen, buf, n);
			h->req_buflen += n;
			h->req_buf[h->req_buflen] = '\0';
			/* search for the string "\r\n\r\n" */
			endheaders = findendheaders(h->req_buf, h->req_buflen);
			if(endheaders)
			{
				h->req_contentoff = endheaders - h->req_buf + 4;
				ProcessHttpQuery_upnphttp(h);
			}
		}
		break;
	case 1:
		n = recv(h->socket, buf, 2048, MSG_DONTWAIT);
		if(n<0)
		{
			syslog(LOG_ERR, "recv (state1): %m");
			h->state = 100;
		}
		else if(n==0)
		{
			syslog(LOG_WARNING, "connection closed inexpectedly");
			h->state = 100;
		}
		else
		{
			/*fwrite(buf, 1, n, stdout);*/	/* debug */
			h->req_buf = (char *)realloc(h->req_buf, n + h->req_buflen);
			if(!h->req_buf){
				h->state = 100;
				break;
			}
			memcpy(h->req_buf + h->req_buflen, buf, n);
			h->req_buflen += n;
			if((h->req_buflen - h->req_contentoff) >= h->req_contentlen)
			{
				ProcessHTTPPOST_upnphttp(h);
			}
		}
		break;
	default:
		syslog(LOG_WARNING, "unexpected state (%d)", h->state);
		break;
	}
}
#ifdef ENABLE_EVENTS
static const char httpresphead[] =
	"%s %d %s\r\n"
	"Content-Type: %s\r\n"
	"Connection: close\r\n"
#ifdef CONFIG_CERTIFICATION
	"DATE: %s\r\n"
	"%s"
#endif
	"Content-Length: %d\r\n"
	"Server: miniupnpd/1.0 UPnP/1.0\r\n"
#ifdef CONFIG_CERTIFICATION
	"Ext:\r\n"
#endif
	;	/*"\r\n";*/
#else
static const char httpresphead[] =
	"%s %d %s\r\n"
	"Content-Type: text/xml; charset=\"utf-8\"\r\n"
	"Connection: close\r\n"
	"Content-Length: %d\r\n"
	"Server: miniupnpd/1.0 UPnP/1.0\r\n"
	"Ext:\r\n"
	"\r\n";
#endif
/*
		"<?xml version=\"1.0\"?>\n"
		"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
		"s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
		"<s:Body>"

		"</s:Body>"
		"</s:Envelope>";
*/
/* with response code and response message
 * also allocate enough memory */
void
BuildHeader_upnphttp(struct upnphttp * h, int respcode,
                     const char * respmsg,
                     int bodylen)
{
	int templen;
#ifdef CONFIG_CERTIFICATION
	time_t t;
	char date[30];
#endif
	if(!h->res_buf)
	{
		templen = sizeof(httpresphead) + 128 + bodylen;
		h->res_buf = (char *)malloc(templen);
		if(!h->res_buf)
			return ;
		h->res_buf_alloclen = templen;
	}
#ifdef CONFIG_CERTIFICATION
	memset(date,0,sizeof(date));
 	time(&t);
	rfc822_time_buf(date,t);
#endif
#ifdef ENABLE_EVENTS
#ifdef CONFIG_CERTIFICATION
    if(h->respflags & FLAG_ICON)
		h->res_buflen = snprintf(h->res_buf, h->res_buf_alloclen,
	                         httpresphead, h->HttpVer,
	                         respcode, respmsg, 
	                         "image/png",date,
	                         (h->respflags&FLAG_LANGUAGE)?"Content-Language: en\r\n":"",
	                         bodylen);
	else
		h->res_buflen = snprintf(h->res_buf, h->res_buf_alloclen,
	                         httpresphead, h->HttpVer,
	                         respcode, respmsg, 
	                         (h->respflags&FLAG_HTML)?"text/html":"text/xml;charset=\"utf-8\"",
	                         date,
	                         (h->respflags&FLAG_LANGUAGE)?"Content-Language: en\r\n":"",
	                         bodylen);
#else
	h->res_buflen = snprintf(h->res_buf, h->res_buf_alloclen,
	                         httpresphead, h->HttpVer,
	                         respcode, respmsg, 
	                         (h->respflags&FLAG_HTML)?"text/html":"text/xml",
	                         bodylen);
#endif
	/* Additional headers */

	if(h->respflags & FLAG_SID) {
		h->res_buflen += snprintf(h->res_buf + h->res_buflen,
		                          h->res_buf_alloclen - h->res_buflen,
		                          "SID: %s\r\n", h->req_SID);
	}
	if(h->respflags & FLAG_TIMEOUT) {
		h->res_buflen += snprintf(h->res_buf + h->res_buflen,
		                          h->res_buf_alloclen - h->res_buflen,
		                          "Timeout: Second-");
#ifdef CONFIG_CERTIFICATION
		if( h->req_Timeout != 0xFFFFFFFF) {
#else
		if(h->req_Timeout) {
#endif
			h->res_buflen += snprintf(h->res_buf + h->res_buflen,
			                          h->res_buf_alloclen - h->res_buflen,
			                          "%d\r\n", 
#ifdef CONFIG_CERTIFICATION
									  (h->req_Timeout < 1800)?1800:h->req_Timeout);
#else
										h->req_Timeout);	
#endif
		} else {
			h->res_buflen += snprintf(h->res_buf + h->res_buflen,
			                          h->res_buf_alloclen - h->res_buflen,
			                          "infinite\r\n");
		}
	}
	h->res_buf[h->res_buflen++] = '\r';
	h->res_buf[h->res_buflen++] = '\n';
#else
	h->res_buflen = snprintf(h->res_buf, h->res_buf_alloclen,httpresphead, h->HttpVer, respcode, respmsg, bodylen);
#endif

	if(h->res_buf_alloclen < (h->res_buflen + bodylen))
	{
		h->res_buf = (char *)realloc(h->res_buf, (h->res_buflen + bodylen));
		if(!h->res_buf)
			return;
		h->res_buf_alloclen = h->res_buflen + bodylen;
	}
}

void
BuildResp2_upnphttp(struct upnphttp * h, int respcode,
                    const char * respmsg,
                    const char * body, int bodylen)
{
	BuildHeader_upnphttp(h, respcode, respmsg, bodylen);
	memcpy(h->res_buf + h->res_buflen, body, bodylen);
	h->res_buflen += bodylen;
}

/* responding 200 OK ! */
void BuildResp_upnphttp(struct upnphttp * h,
                        const char * body, int bodylen)
{
	BuildResp2_upnphttp(h, 200, "OK", body, bodylen);
}

void SendResp_upnphttp(struct upnphttp * h)
{
	int n;
//david	
//	n = send(h->socket, h->res_buf, h->res_buflen, 0);
	n = send(h->socket, h->res_buf, h->res_buflen, MSG_DONTROUTE | MSG_NOSIGNAL);	
	if(n<0)
	{
		syslog(LOG_ERR, "send(res_buf): %m");
	}
	else if(n < h->res_buflen)
	{
		syslog(LOG_ERR, "send(res_buf): %d bytes sent (out of %d)",
						n, h->res_buflen);
	}
}

