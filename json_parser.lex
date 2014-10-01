%option outfile="ucoder_json.cpp"
%option c++
%option noyywrap

%{
/*
 * UniversalContainer library.
 * Copyright Jason Denton, 2008,2010.
 * Made available under the new BSD license, as described in LICENSE
 *
 * Send comments and bug reports to jason.denton@gmail.com
 * http://www.greatpanic.com/code.html
 */

#include "ucontainer.h"
#include "buffer.h"
#include "ucio.h"
#include <stdint.h>

#define JSON_DECODE_OPEN_MAP 1
#define JSON_DECODE_CLOSE_MAP 2
#define JSON_DECODE_OPEN_ARRAY 3
#define JSON_DECODE_CLOSE_ARRAY 4
#define JSON_DECODE_KVSEP 5
#define JSON_DECODE_STRING 6
#define JSON_DECODE_NUMBER 7
#define JSON_DECODE_LITERAL 8
#define JSON_DECODE_COMMA 9
#define JSON_DEOCDE_EOF 10 
#define JSON_NEWLINE 11
#define JSON_DECODE_ERROR -1
#define JSON_NO_SYMBOL -2

#define YY_NO_UNISTD_H 1

   static int line_number = 1;

%}

DIGIT [0-9]
INTEGER ([+-]?{DIGIT}+)|(?i:[+-]?nan)|(?i:[+-]?infinity)|(?i:[+-]?inf)
EXPONENT [eE]{INTEGER}
FRACTION "."{DIGIT}+
NUMBER {INTEGER}({FRACTION}{EXPONENT}?)?
	
ECHAR "\\"[nrtfb\\\"]
HEXCHAR "\\u"[0-9a-fA-F]{4}
PCHAR [\[\]\\a-zA-Z0-9<>(){}+ =/!?@#:$%^&*\,\_\.\-'\x80-\xff]
CHARACTER {ECHAR}|{HEXCHAR}|{PCHAR}
STRING "\""{CHARACTER}*"\""
CARRIAGE "\r"
NEWLINE "\n"

LITERAL "true"|"false"|"null"|"TRUE"|"FALSE"|"NULL"	

OPENMAP "{"
CLOSEMAP "}"
OPENARRAY "["
CLOSEARRAY "]"

KVSEP ":"
COMMA ","

%%
{OPENMAP} return JSON_DECODE_OPEN_MAP;
{CLOSEMAP} return JSON_DECODE_CLOSE_MAP;
{OPENARRAY} return JSON_DECODE_OPEN_ARRAY;
{CLOSEARRAY} return JSON_DECODE_CLOSE_ARRAY;
{KVSEP} return JSON_DECODE_KVSEP;
{STRING} return JSON_DECODE_STRING;
{NUMBER} return JSON_DECODE_NUMBER;
{LITERAL} return JSON_DECODE_LITERAL;
{COMMA} return JSON_DECODE_COMMA;
{NEWLINE} line_number++;
<<EOF>> return JSON_DEOCDE_EOF;
[\t\r\n ] //eat whitespace and commas
. return JSON_DECODE_ERROR;
%%

namespace JAD {

  class JSONLexer : public yyFlexLexer
  {
    Buffer* buffer;

    virtual int LexerInput(char*, int);
    virtual size_t LexerInput(char* data, size_t max_size);
  
  public:
    JSONLexer(Buffer*);
    char* get_text();
  };
  
  JSONLexer::JSONLexer(Buffer* buf) : yyFlexLexer(0,0)
  {
    buffer = buf;
  }
 
  int JSONLexer::LexerInput(char* data, int max_size)
  {
    int sent = (int) buffer->copy_out(data,(size_t) max_size);
    return sent;
  }
  
  size_t JSONLexer::LexerInput(char* data, size_t max_size)
  {
    int sent = (int) buffer->copy_out(data,(size_t) max_size);
    return sent;
  }
  
  char* JSONLexer::get_text(void)
  {
    return yytext;
  }

  UniversalContainer unescape_json_string(char* str)
  {
    UniversalContainer uc;
    bool need_wstr = false;
    string s;
    wstring w;
    size_t i = 1; //skip opening "

    while (str[i]) {
      if (str[i] != '\\') {
      	if (str[i] == '"') break;
      	w.push_back(str[i]);
      	s.push_back(str[i]);
      	i++;
      	continue;
      } 
      i++; 
      switch(str[i]) {
      case '/' :
      case '\\' :
      case '"' : 
      	w.push_back(str[i]);
      	s.push_back(str[i]);
      	break;
      case 'n' :
	      w.push_back('\n');
        s.push_back('\n');
	     break;
      case 't' :
	      w.push_back('\t');
        s.push_back('\t');
	      break;
      case 'r' :
      	w.push_back('\r');
      	s.push_back('\r');
      	break;
      case 'b' :
      	w.push_back('\b');
      	s.push_back('\b');
      	break;
      case 'f' :
      	w.push_back('\f');
      	s.push_back('\f');
      	break;
      case 'u' :
        unsigned int tmp;
      	sscanf(str+i+1,"%02X",&tmp);
      	w.push_back(wchar_t(tmp));
      	need_wstr = true;
      	i += 2;
      	break;
      }
      i++;
    }
    if (need_wstr) uc = w;
    else {
       if (s.length() == 1) uc = s[0];
       else uc = s;
    }
    return uc;
  }
  
  UniversalContainer uc_decode_json(JSONLexer* lex, int symbol = JSON_NO_SYMBOL)
  {
    UniversalContainer uc;
    int idx = 0;
    string key;
    string value;
    UniversalContainer tmp;

    try {
    if (symbol == JSON_NO_SYMBOL) symbol = lex->yylex();
    switch (symbol) {
    case JSON_DECODE_OPEN_MAP :
      symbol = lex->yylex();
      uc.init_map();
      while (symbol != JSON_DECODE_CLOSE_MAP) {
	     if (symbol != JSON_DECODE_STRING) 
	       throw ucexceptionuc(uc,uce_Deserialization_Error);
	     tmp = unescape_json_string(lex->get_text());
	     key = static_cast<string>(tmp);
	     symbol = lex->yylex();
	     if (symbol != JSON_DECODE_KVSEP)
	       throw ucexceptionuc(uc,uce_Deserialization_Error);
	     uc[key] = uc_decode_json(lex);
	     symbol = lex->yylex();
	     if (symbol == JSON_DECODE_COMMA)
	       symbol = lex->yylex();
      }
      return uc;
    case JSON_DECODE_OPEN_ARRAY :
      uc.init_array();
      symbol = lex->yylex();
      while (symbol != JSON_DECODE_CLOSE_ARRAY) {
	      uc[idx] = uc_decode_json(lex,symbol);
        symbol = lex->yylex();
        if (symbol == JSON_DECODE_CLOSE_ARRAY) break;
        if (symbol != JSON_DECODE_COMMA) throw ucexceptionuc(uc,uce_Deserialization_Error);
        symbol = JSON_NO_SYMBOL;
	      idx++;
      }
      return uc;
    case JSON_DECODE_STRING :
      uc = unescape_json_string(lex->get_text());
      return uc;
    case JSON_DECODE_NUMBER :
    case JSON_DECODE_LITERAL :
      uc.string_interpret(lex->get_text());
      return uc;
      break;
    case JSON_DECODE_ERROR:
    default :
      UniversalContainer uce = ucexceptionuc(uc,uce_Deserialization_Error);
      uce["input line"] = line_number;
      throw uce;
    } //end switch
    } catch(UniversalContainer uce) {
      uce["input line"] = line_number;
      throw uce;
    }
    return uc; //should never happen
  }

  UniversalContainer uc_decode_json(Buffer* buf)
  {
    line_number = 0; //lex->lineno appears to be broken, so we have this hack...
    JSONLexer* jl = new JSONLexer(buf);
    UniversalContainer uc = uc_decode_json(jl);
    delete jl;
    return uc;
  }

  void escape_char(string& str, char c)
  {
    if (c == '"') {
      str.push_back('\\');
      str.push_back('"');
    }

    else if (c == '\n') {
      str.push_back('\\');
      str.push_back('n');
    }     
    else if (c == '\t') {
      str.push_back('\\');
      str.push_back('t');
    }
    else if (c == '\f') {
      str.push_back('\\');
      str.push_back('f');
    }
    else if (c == '\r') {
      str.push_back('\\');
      str.push_back('r');
    }
    else if (c == '\b') {
      str.push_back('\\');
      str.push_back('b');
    }
    else if (c == '\\') {
      str.push_back('\\');
      str.push_back('\\');
    }
    else str.push_back(c);
  }
  
  string escape_string(const wstring& orig)
  {
    string esc;
    char buf[8];

    for (size_t i = 0; i < orig.length(); i++) {
      if (orig[i] > 255) {
	     sprintf(buf,"\\u%04X",(int)orig[i]);
	     esc.append(buf);
      }
      else escape_char(esc,static_cast<char>(orig[i]));
    }
    return esc;
  }

  string escape_string(const string& orig)
  {
    string esc;

    for (size_t i = 0; i < orig.length(); i++) 
      escape_char(esc,static_cast<char>(orig[i]));
    
    return esc;
  }

  extern std::map<UniversalContainerType, UniversalContainerTypeAdapter*> UniversalContainerAdapter;

  void uc_encode_json(const UniversalContainer& uc, Buffer* buffer)
  {
    UniversalMap::iterator miter;
    UniversalMap::iterator mend;
    UniversalArray::iterator viter;
    UniversalArray::iterator vend;

    bool first_pass = false;
    string tmp;
    UniversalContainerType type = uc.get_type();

    switch(type) {
    case uc_Map :
      buffer->put('{');
      mend = map_end(uc);
      for(miter = map_begin(uc); miter != mend; miter++) {
      	if (miter->first[0] == '#') continue; //skip metadata
      	if (first_pass) buffer->put(',');
      	buffer->put('"');
      	tmp = escape_string(miter->first);
      	buffer->put_data(tmp.c_str(),tmp.length());
      	buffer->put('"');
      	buffer->put(':');
      	uc_encode_json(miter->second,buffer);
      	first_pass = true;
      }
      buffer->put('}');
      break;
    case uc_Array :
      buffer->put('[');
      vend = vector_end(uc);
      for(viter = vector_begin(uc); viter != vend; viter++) {
      	if (first_pass) buffer->put(',');
      	uc_encode_json(*viter,buffer);
      	first_pass = true;
      }
      buffer->put(']');
      break;    
    case uc_String :
      buffer->put('"');
      tmp = escape_string(static_cast<string>(uc));
      buffer->put_data(tmp.c_str(),tmp.length());
      buffer->put('"');
      break;
    case uc_WString :
      buffer->put('"');
      tmp = escape_string(static_cast<wstring>(uc));
      buffer->put_data(tmp.c_str(),tmp.length());
      buffer->put('"');
      break;
    case uc_Character :
      buffer->put('"');
      buffer->put(static_cast<char>(uc));
      buffer->put('"');
      break;
    default :      
     // tmp = uc.to_string();
      tmp = UniversalContainerAdapter[uc.get_type()]->to_json(uc.cast_or_throw(uc.get_type()));
      buffer->put_data(tmp.c_str(),tmp.length());
    } //end type switch
  }

  void uc_encode_pretty_json(const UniversalContainer& uc, Buffer* buffer, int depth, int indent)
  {
    UniversalMap::iterator miter;
    UniversalMap::iterator mend;
    UniversalArray::iterator viter;
    UniversalArray::iterator vend;

    bool first_pass = false;
    string tmp;
    UniversalContainerType type = uc.get_type();

    switch(type) {
    case uc_Null :
    case uc_Integer :
    case uc_Real :
    case uc_Boolean :
      tmp = uc.to_string();
      buffer->put_data(tmp.c_str(),tmp.length());
      break;
    case uc_Map :
      buffer->put('{');
      mend = map_end(uc);
      for(miter = map_begin(uc); miter != mend; miter++) {
      	if (miter->first[0] == '#') continue; //skip metadata
      	if (first_pass) buffer->put(',');
        buffer->put('\n');
        for (int dc = 0; dc < (depth+1)*indent; dc++) buffer->put(' ');
        	buffer->put('"');
        	tmp = escape_string(miter->first);
        	buffer->put_data(tmp.c_str(),tmp.length());
        	buffer->put('"');
        	buffer->put(' ');
        	buffer->put(':');
        	buffer->put(' ');
	        uc_encode_pretty_json(miter->second,buffer,depth+1,indent);
	        first_pass = true;
        }
      buffer->put('\n');
      for (int dc = 0; dc < depth*indent; dc++) buffer->put(' ');
      buffer->put('}');
      break;
    case uc_Array :
      buffer->put('[');
      vend = vector_end(uc);
      for(viter = vector_begin(uc); viter != vend; viter++) {
      	if (first_pass) buffer->put(',');
      	uc_encode_pretty_json(*viter,buffer,depth,indent);
      	first_pass = true;
      }
      buffer->put(']');
      break;    
    case uc_String :
      buffer->put('"');
      tmp = escape_string(static_cast<string>(uc));
      buffer->put_data(tmp.c_str(),tmp.length());
      buffer->put('"');
      break;
    case uc_WString :
      buffer->put('"');
      tmp = escape_string(static_cast<wstring>(uc));
      buffer->put_data(tmp.c_str(),tmp.length());
      buffer->put('"');
      break;
    case uc_Character :
      buffer->put('"');
      buffer->put(static_cast<char>(uc));
      buffer->put('"');
      break;
    default :    
      tmp = UniversalContainerAdapter[uc.get_type()]->to_json(uc.cast_or_throw(uc.get_type()));
      buffer->put_data(tmp.c_str(),tmp.length());
      break;
    } //end type switch

    if (!indent) { //top level
      buffer->put('\n');
      buffer->put('\0');
    }
  }

  Buffer* uc_encode_json(const UniversalContainer& uc)
  {
    Buffer* buffer = new Buffer;
    uc_encode_json(uc, buffer);
    buffer->put('\0');
    buffer->rewind();
    return buffer;
  }

} //end namespace
