___________                       _____.__        
\__    ___/___________  ___  ____/ ____\__| ____  
  |    |  \_  __ \__  \ \  \/  /\   __\|  |/ ___\ 
  |    |   |  | \// __ \_>    <  |  |  |  / /_/  >
  |____|   |__|  (____  /__/\_ \ |__|  |__\___  / 
                      \/      \/         /_____/ 
 
Here is traxfig version 2.11.

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

From T.Siegenthaler@massey.ac.nz Tue Nov 23 04:38:24 1993
Received: from cc-server4.massey.ac.nz by babbage.imag.fr with SMTP id AA25926
  (5.65c8/IDA-1.4.4 for <Thierry.Grellier@ufrima.imag.fr>); Tue, 23 Nov 1993 04:38:00 +0100
Received: by cc-server4.massey.ac.nz (5.64+IDA-1.3.1)
	  id AA00726; Tue, 23 Nov 93 16:42:04 +1300
From: "T.D. Siegenthaler" <T.Siegenthaler@massey.ac.nz>
Message-Id: <9311230342.AA00726@cc-server4.massey.ac.nz>
Date: Tue, 23 Nov 93 16:39:44 +1200
To: Thierry Grellier <Thierry.Grellier@ufrima.imag.fr>
Subject: Re: Trax rules
In-Reply-To: Your letter of Mon, 22 Nov 1993 15:03:06 +0100
X-Mailer: NETMAIL/PC Version 1j
Status: OR


Rules to Trax
Invented by David Smith of Christchurch, New Zealand 1980
Copyright David Smith 

Trax is a game played with tiles. These tiles are red with white and black
straight sections on one side and white and black curved sections on the
other:


        _____________     _____________
       |.....| |.....|   |.....| |.....|
       |.....| |.....|   |..../ /......|
       |.....| |.....|   |___/ /.......|
       |#############|   |____/....####|
       |.....| |.....|   |......####...|
       |.....| |.....|   |.....##......|
       |_____|_|_____|   |_____##______|


As these are too difficult to draw in ascii characters, This method has 
been developed to show the tiles graphically:

            + o +            + o +           + o + x +    Two tiles 
            x   x            o / x           x \ o   o    side by side
            + o +            + x +           + x + x +

Notice the similarity to the above tiles.
As this descriptive form can be hard to read, I recommend you make some
tiles from card, draw them on graph paper or get the real thing.


Rules:
1  Trax is played with identical tiles as described above on a flat
     surface.
2  Two players decide who shall be white and who shall be black.
3  White always plays first (there is no advantage).
4  Each player in turn plays one tile either side up, adjacent to any already
     played tile such that the colours line up.
5  Each player may join track of either or both colours at any turn.
6  If any played tile results in an adjacent space or spaces that has two
     tracks of the same colour entering it a `forced' tile is played so as to
     join the two tracks together. If this tile creates another space with
     two tracks of the same colour entering then a second forced tile is
     played. The players turn ends when all forced tiles have been played.
   Note: Think of this not so much as being forced to play tiles, but an
     oportunity to play more than one tile.
7  If a tile or forced tile is played that results in in a space with three
     or four tracks of the same colour entering, the move is illegal and must
     be taken back and replayed (at no penalty).
8  The winner is the player whose track first forms a LOOP or a LINE.
9  A loop is any path of track that connects to itself, whatever its shape.
10 A line is a continuous path that connects oppisite and outermost edges
     of the layout, at least 8 rows of tile across or down.
11 If a loop or a line is formed by both colours during either players turn,
     the game is won by that player.
12 The playing area may not be extended beyond 8 rows wide and 8 rows deep
     (Regular Trax).
13 If a win is not achieved after all 64 tiles have been played, the game is
     drawn.
14 Missed forced moves should be played as soon as noticed, at no penalty
     to either player.

SUPERTRAX
15  Draws are not permitted.
16  The playing area may extend beyond the playing limit of 8x8 (rules 12
    and 13 above are canceled).
    Note: obviously, more than one set of tiles may be required. The largest
    game I know was 255 tiles, average is about 80.

NOTATION
17  The game is recorded as viewed by white.
18  Three symbols are used to record the first tile played.
19  Forced tiles are not recorded as these are self evident.
20  First symbol - letters are used to denote the column played reading
      left to right AFTER the tile is played.
21  Second symbol - numbers are used to denote the row played reading top
      to bottem AFTER the tile is played.
22  The above order is reversed to distinguish a tile played ABOVE rather
      than TO THE LEFT of the top left corner.           
                                                        1A
                                        ie              __
                                                       |  |
                                                   A1  |__|  B1

                                                        A2


23  Third symbol - S is used whenever straights are played. 
      C is used whenever curves is played as the first tile or against two
      edges. Ohterwise U, D, L or R are used when the track extended is
      turned up, down, left or right respectively.
24  For notational purposes, the first tile must be played this way up:

         + o +                       + o +
         x   x  if straights,  or    o / x  if curves.
         + o +                       + x +


examples:


     From this position ( known as an L for its shape )

                + o + x + o +
                x \ o \ x   x
                + x + o + o +
                        x \ o
                        + x +



 Black plays: 
                + o + x + o +             + o + x + o +
                x \ o \ x   x             x \ o \ x   x
                + x + o + o +             + x + o + o +
       here  -> x / o   x \ o             x / o / x \ o
                + o +   + x +             + o + x + x +

                                        forcing ^

This creates two half loops, one on the left and one at the bottem.
White defends one:

                + o + x + o +
                x \ o \ x   x
                + x + o + o +
                x / o / x \ o
                + o + x + x + 
                        o \ x
                        + o +



Black plays
 + o + o + x + o +              + o + o + x + o +
 o / x \ o \ x   x              o / x \ o \ x   x    resulting in a loop
 + x + x + o + o +   forcing:   + x + x + o + o +    win for black.
     x / o / x \ o            > o \ x / o / x \ o
     + o + x + x +              + o + o + x + x +
             o \ x                          o \ x
             + o +                          + o +

This is an example of a simple two stage win. Most wins in Trax require the
setting up of such multiple stage wins.


Clues to strategy:
Defending loops tends to straighten them into lines.
Defending lines tends to bend them into loops.
Don't attack too soon - you use up your corners and your opponents defence
  will give them corners.
If you don't know what to do, DON'T attack. Kill a corner instead.
Approximate guides as to who has the best position:
 The person with the most corners and the most line segments is probably
 doing better.




Here is a sample game, by two people who have been playing for years. It is
not a beginners game, but the commentary should give you some insight as to 
the strategy employed here. I have put a dot in the first tile to make it
easier to find. This is not normally done.

White (o)  Donald Bailey     (DBailey@massey.ac.nz)
Black (x)  Tom Siegenthaler  (TSiegenthaler@massey.ac.nz)

                 white      black
                   o          x

+ o +
x . x            1 A1S
+ o +

+ x + o +
o \ x . x         1 A1S   2 A1U
+ o + o +

+ x + o +
o \ x . x         1 A1S   2 A1U
+ o + o +         3 B2R             A less common though safe
    x \ o                           opening
    + x +

+ x + o +
o \ x . x         1 A1S   2 A1U
+ o + o +         3 B2R   4 A2S      Another unconvential move.
x   x \ o                            Still safe
+ o + x +


+ o + x +                         White sets up an opening trap where 
o / x / o                         there is only one defence. The trap is:
+ x + o +                           C1U  D1S  any  D5R  any  A5L  any  win
o \ x . x         1 A1S   2 A1U          C2U  1Al  any  D1U  any  win
+ o + o +         3 B2R   4 A2S          C2D  A4L  any  D4D  any  win
x   x \ o         5 B1R
+ o + x +


+ o + x + x +                 
o / x / o \ x     1 A1S   2 A1U
+ x + o + o +     3 B2R   4 A2S   Black chooses the only safe defence
o \ x . x \ o     5 B1R   6 C3D
+ o + o + x +   
x   x \ o \ x            
+ o + x + o +

+ o + x + x + o +               
o / x / o \ x \ o   1 A1S   2 A1U
+ x + o + o + x +   3 B2R   4 A2S   White continues the attack, also forming
o \ x . x \ o \ x   5 B1R   6 C3D   an edge along the top.
+ o + o + x + o +   7 D3S             (D1R any 1AL any win)
x   x \ o \ x   x          
+ o + x + o + o +


+ o + x + x + o +               
o / x / o \ x \ o   1 A1S   2 A1U
+ x + o + o + x +   3 B2R   4 A2S   Black makes the required counter attack
o \ x . x \ o \ x   5 B1R   6 C3D   to avoid loosing to the edge.
+ o + o + x + o +   7 D3S   8 A4S
x   x \ o \ x   x          
+ o + x + o + o +
x   x / o / x
+ o + o + x +

    + o + x + x + o +               
    o / x / o \ x \ o   1 A1S   2 A1U
+ x + x + o + o + x +   3 B2R   4 A2S   This thoroughly kills the black
o   o \ x . x \ o \ x   5 B1R   6 C3D   attack and leaves two threats: the
+ x + o + o + x + o +   7 D3S   8 A4S   edge along the top and a 4-stage on
o \ x   x \ o \ x   x   9 A2S           the right.
+ o + o + x + o + o +
    x   x / o / x
    + o + o + x +


    + x + o + o + x +
    x / o / x \ o \ x
    + o + x + x + o +               
    o / x / o \ x \ o   1 A1S   2 A1U   Black caps off the edge while making
+ x + x + o + o + x +   3 B2R   4 A2S   an attack. This move is actually 
o   o \ x . x \ o \ x   5 B1R   6 C3D   faulty as it does not defend the 
+ x + o + o + x + o +   7 D3S   8 A4S   white 4-stage.
o \ x   x \ o \ x   x   9 A2S  10 C1R
+ o + o + x + o + o +
    x   x / o / x
    + o + o + x +

(If the drawing gets too large, the spaces can be removed. They are only 
in there for readability)

to speed things up:

11  F2S                 White defends both the attack and the edge while
                        maintaining the 3-stage threat on the bottom right.
                        This lets black off the hook -
                        F1S would have won for white:
                         F1S  any  D7S  E6C  F6D  any  win
                                        E7D  F7S  any  win
                    
                12 G2D  Let off the hook, black kills the 3-stage while
                        setting up his own 4-stage across the top:
                         B1L  any  C1R  D1U  G1S  any  win
                                        any  G2U  any  win

13  C1S                 Destroys the threat by killing a critical corner.
                    
                14 D7R  Black attacks, hoping to use the corner to later
                        link up to the horizontal or vertical line.
           
15 E7D                  The only safe defence. E6S loses to:
                         C8L  any  E8D  any  win

                16 E1S  Black realises that white also has a strong vertical
                        line prospect. This move continues pushing the black
                        line while slowing down the white line. It also
                        creates a black L (G1U).

17 F7D                  White attacks, partially turning back the black line

                18 F8R  Black counterattacks with an unconnected line (H5S).

19 E1R                  White turns back the other end of the line threat by
                        linking it back to the L. This kills the Horizontal
                        line that was building, and also sets up an L in the
                        top left corner (B1R).

                20 E1R  This defuses the white L while forming a black L
                         (G1U).

21 H5S                  White makes an unconventional defence of the new L.
                        In this case the conventional methods don't work.
                        This move sets up a 4-stage threat along the bottom
                        and right hand sides.
                          H6S  any  C11L  any  H11R  any  win

                22 G2S  Black reforms the line attack and temporarily
                        defuses the 4-stage by providing a counter attack.
                         (H3U)

23 F1R                  White defends the line threat.

                24 G1S  This allows black to straighten the line again later
                        and sets up a 4-stage loop threat:
                         H1U  any  I6U  H5C  I5S  any  win
                                        I5R  I4S  any  win

25 I6D                  White kills a critical cornerwhile setting an L (I5R)
                        Note that black has a counter attack to the other
                        3-stage threat (H7S, H4S)               

                26 H3D  Black kills both threats anyway, setting up his own
                        2-stage (H1U) and 3-stage (J6S  any  J3R  any win).

27 H7S                  White kills both threats while setting up 3 more of
                        his own; an L, an edge and a loop line threat:
                         I11D  any  win
                         C12L  any  H12R  any  win
                         D3S  any  D1l  any  win

                28 J7S  This kills the L and defuses the loop/line threat
                        but ignores the edge. Defending all three is quite
                        difficult. Possibly 28 F12R may work.

29 C12L         30 B10S White activate the the edge to win
31 H12R



(One day I'll get around to putting a diagram for the full game here)


Tom

-------------------------------------------------------------------------------

Technical part now:

+ translating messages

The end of each message is detected by the empty line which follows it. That
allows to have messages longer than one line. If the char $ ends the message,
it means that there wont't be a carriage return after the message.

+ creating a new pattern for tiles drawing

The tile must be in the same order tham in other .tfm files, with a 16x16 max size
for each. They must be separated by an empty line.

+ bug report
	if you fix a bug in this program (and you will it hasn't been tested
very well), please send me a report. I'm reachable at Thierry.Grellier@imag.fr
until october 1994, and I may be reachable from october 1994 to june 1995 at 
Thierry.Grellier@ufrima.imag.fr, but I will have less free time (army :-(

+ copyright
	You can freely reuse this code provided you mention where it does come
from.

+ for whose who have read until there, a funny test to type :

> traxfig A1C 1AR 1AR 1AR B1D C1D D1D B4U C4U D2S

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
	Improvement of postscript output by Richard.
