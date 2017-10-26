# AQ for Problem Solving (AQ-PS)
AQ-PS is a fork of the Go/Baduk/Weiqi playing bot [AQ](https://github.com/ymgaq/AQ). AQ-PS is tweaked so that it does not play full games from the beginning to the endgame, but plays only locally, especially for life and death problems at a corner or an edge.  

Currently AQ-PS is not very good at playing high level problems. Still, I hope this bot could be helpful to some kyu players like me.  

### Use case
For better understanding life and death problems found on books. Sometimes you may not fully understand the answer provided by books, and wonder what happens if you play here or why playing there won't work. The bot could help you go through variations that are not included in the answer, as deep as you like.  

For reviewing life and death situations occurred in your real games.

### Download

https://github.com/geovens/AQ-PS/releases

### Usage

Use it in Sabaki or GoGui.  
There are some settings in aq_config.txt, among which and you might care about -thread count and -byoyomi[sec] (seconds per move).

### Keypoint
There is a 'keypoint' on the board, around which AQ-PS will pay full attention to, instead of finding big open moves or thinking about the whole-board. By default the keypoint is automatically set around the middle of existing stones on the board, and should work fine for most life and death problems. You can also manually set the position of the keypoint using GTP command 'keypoint'. For example, 'keypoint a1' sets the bottom left corner as the only area that the bot will be interested in. Manually setting the position of the keypoint is necessary when there are many stones all across the board, as in a real game.

More precisely, the vertices in a 9x9 area centered at the keypoint worth 1 point each, while all other vertices on the board worth 0 point. Based on this, the bot will try to find a move that maximize the expected score. Sometimes you might need to be creative on how to use this bot for your purpose in a particular situation.

### Ko Avoiding

By default, the bot tries to avoid ko when searching the best local sequence. You can use GTP command 'avoidko 0' or 'avoidko 1' to turn this feature off or on.

### Benchmark
Graded Go Problems for Beginners Volume 4, section 3 life and death problems: the first move is played the same as the answer 46/60 (77%) of the times for the first 60 problems (more to be tested). (i7-4790 CPU, 5 threads, 20s per move. Hardware and time matters a lot.) 

Note that, sometimes when the bot plays a move different than the answer, it does not necessarily mean that the bot is wrong. 

### Requirement
- OS: 64-bit Windows  
- CPU: CPU with SSE 4.2 


