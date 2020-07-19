#pragma once

#define unused(SYM) SYM __attribute__((unused))
#define echo_stream(__expr__) " " << #__expr__ << ":{" << __expr__ << "}"
