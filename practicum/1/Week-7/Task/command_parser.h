#ifndef __COMMAND_PARSER_H__
#define __COMMAND_PARSER_H__

#include <istream>
#include "corporation.h"

bool command_parser (std::istream &in, std::ostream *out, corporation *(&corp), std::ostream *err, bool recover_from_errors);

#endif
