# AQ for Problem Solving (AQ-PS)
AQ-PS is a fork of the Go/Baduk/Weiqi playing bot [AQ](https://github.com/ymgaq/AQ). AQ-PS is tweaked so that it does not play full games from the beginning to the endgame, but plays only locally, especially for life and death problems at a corner or an edge.  

Currently AQ-PS is not very good at playing high level problems. Anyway, it could still be very helpful to some kyu players like me.  

### Use case
For better understanding life and death problems found on books. Sometimes you may not fully understand the answer provided by books, and wonder what happens if you play here or why playing there won't work. The bot could help you go through variations that are not included in the answer, as deep as you like.  

For reviewing life and death situations occurred in your real games.

### Benchmark
I tested AQ-PS with problems on books. For a problem, if the first move generated by AQ-PS is the same as the answer provided by the book or the errata, it's counted as correct. The correct rate is as follows:

1001 Life and Death Problems:  
	956/1001 (95.5%), using i7-7700 CPU, 3 threads, 10 seconds per move.  
  
Graded Go Problems for Beginners Volume 4, section 4 life and death problems:  
	96/124 (77%), using i7-7700 CPU, 3 threads, 20 seconds per move.  
  
Note that, sometimes when the bot plays a move different than the answer, it does not necessarily mean that the bot is wrong.  

### Download

https://github.com/geovens/AQ-PS/releases

### Usage

AQ-PS is a GTP engine. Use it in [Sabaki](http://sabaki.yichuanshen.de/) or [GoGui](https://sourceforge.net/projects/gogui/).  
There are some settings in aq_config.txt, among which you should care only about "-thread count" and "-byoyomi[sec]". The former sets how many CPU threads to use (more than 3 is not recommended), and the latter sets how many thinking time AQ-PS takes per move.

### Keypoint
There is a 'keypoint' on the board, around which AQ-PS will pay full attention to, instead of finding big open moves or thinking about the whole-board. By default the keypoint is automatically set around the middle of existing stones on the board, and should work fine for most life and death problems. You can also manually set the position of the keypoint using GTP command 'keypoint'. For example, 'keypoint A1' sets the bottom left corner as the only area that the bot will be interested in. Manually setting the position of the keypoint is necessary when there are many stones all across the board, as in a real game.

More precisely, the vertices in a 9x9 area centered at the keypoint worth 1 point each, while all other vertices on the board worth 0 point. Based on this, the bot will try to find a move that maximize the expected score. 

### Simulation of Infinite Ko Threats
You can use the GTP command 'allow_illegal_ko black' to allow black to play illegal ko (while white is not allowed to play illegal ko), and 'allow_illegal_ko white' to allow white to play illegal ko (while black is not allowed to play illegal ko). Illegal ko is the move that immediately retakes a ko which is not allowed by rule of the game. This feature is by default disabled, and you can use the GTP command 'allow_illegal_ko none' to turn off this feature after turning it on.

This feature is meant to be used to simulate infinite ko threats for one player, and might be needed to force AQ-PS to find better solutions for some problems than creating kos. For example, in a problem that black should be able to kill white unconditionally without creating a ko, AQ-PS might find it more benifiting for black to creat a ko because when there are no ko threat black can win the ko anyway. For such a problem, if you allow white to play illegal kos so black couldn't win any ko he would create, AQ-PS will have to find other solutions.  

Be creative on how to use the feature of allowing illegal kos and the feature of keypoint to find better answers for each specific problem!

The benchmark result showed above is calculated in such a way that, a problem is considered to be answered correctly if a correct move is generated by either allowing black or white to play illegal kos.

### Requirement
- OS: 64-bit Windows  
- CPU: CPU with AVX


