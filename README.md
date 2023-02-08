The base of this code is very old and really needs to be updated to modern C.

TRAXFIG:

Traxfig is a filter program to have a display of a trax game. This display
can either be in ascii or in PostScript (Thanks to Uwe G. Wilhelm and
Richard Rognlie).

It can read moves from a file. It trys to detect move notation
either directly (ex: A1C) or by considering a move as the number of the move
followed by the move notation (ex: ...5 A1C, 6 1AS). That's a clever way to
build a whole game view from a commented game. Try this file as an input to
see what is gives ! (type traxfig -n readme.traxfig)

You can also enter move as parameter : traxfig A1C A1D...

There are several ascii display that you can choose with -f <name of tile font>.
You can add yours in tfm/ directory. One allows to watch when tiles have been
played, even forced tile with. Try "traxfig -f numbered.tfm A1S A1U B2R A2S B1R".

EMACS

You can use it with emacs. Here is how Nelson Minar does :
The important thing is the function traxfig-buffer - everything else is a
VM keybinding.

(defun traxfig-buffer ()
  "traxfig the buffer into postscript"
  (interactive)
  (shell-command-on-region (point-min) (point-max) 
	"traxfig -ps | ghostview -"))

(defun my-vm-setup-keys ()
  (define-key vm-mode-map "&" 'traxfig-buffer))

(add-hook 'vm-mode-hook 'my-vm-setup-keys)


CAUTION : you must always have a file named traxfig.msg with all the messages.

COMPILATION :
just type : source install.traxfig or csh install.traxfig if you use
another shell than c-shell


USAGE:

traxfig [-v] [-n] [-l <messages>] [-f <format>] [-o { '=' | <ouput file> }]
        [-ps] [ <input file> | <move sequence> ]
-v               : verbose
-n               : move detected when number followed by move notation
-f <format>      : drawing style of tiles
-o =             : output to file <input file name>.out or traxfig.out if
                   there is no input file.
-o <output file> : output to file <output file>
<move sequence>  : ex. "A1S 1AR B1U"
-l <message>     : error messages file (for foreign languages)
-ps              : output in PostScript (no verbose)
-a               : all tiles numbered (with numbered.tfm file) or with -ps

-------------------------------------------------------------------------------

Technical part now:

+ translating messages

The end of each message is detected by the empty line which follows it. That
allows to have messages longer than one line. If the char $ ends the message,
it means that there wont't be a carriage return after the message.

+ creating a new pattern for tiles drawing

The tile must be in the same order tham in other .tfm files, with a 16x16 max size
for each. They must be separated by an empty line.

------------------------------------------------------------------------
version 2.01
	added absolute paths to find traxfig files

version 2.02
	added the definition of SEEK_END if needed
	added debug option
	added installation file

version 2.03
	included Uwe G. Wilhelm's PostScript output

version 2.04
	read inputs from stdin which allows piping (suggested by Uwe)

version 2.05
	remove a bug of memory allocation for ascii printing

version 2.06
	minor changes everywhere exepted in the code !

version 2.07
	new tiles font, add directories for sources, tile fonts, messages

version 2.08
	add numbered tile display in ascii

version 2.09
	Richard has added numbered tile display in PostScript.

version 2.10
	Minor changes to distinguish played tiles from forced tiles when
	they are numbered.
	
version 2.11
	Add -s output format to output in slugbugblue format. Made by Martin M. S. Pedersen
	

version 2.11
	Improvement of postscript output by Richard.
