infocmp -x xterm-256color | egrep -v "(^XM)" > xterm-256color-xm
infocmp -x xterm-1006 | egrep "(^XM)" >> xterm-256-color-xm
tic -x xterm-256-color-xm
