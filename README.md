# AQ for Problem Solving (AQ-PS)

AQ-PS is a fork of [AQ](https://github.com/ymgaq/AQ). AQ-PS is a not a bot that plays whole games of Go, but a bot that plays life and death problems.  
Currently AQ-PS is not strong at playing problems of high advanced level.  

Pre-compiled executables might be released if AQ-PS could be strong someday, or could turn out to be strong enough to be helpful for kyu players like me.

## Usage

### Keypoint
There is a 'keypoint' on the board, around which AQ-PS will pay full attention to, instead of finding big open moves or thinking about the whole-board. By default the keypoint is automatically set around the middle of existing stones on the board (not implemented yet). You can also set the position of the keypoint using GTP command 'keypoint'. For example, 'keypoint a1' sets the bottom left corner as the only area that the bot will be interested in.

### Use case

For better understanding life and death problems found on books. Sometimes you may not fully understand the answer provided by books, and wonder what happens if you play here or why playing there won't work. The bot could help you go through variations that are not included in the answer, as deep as you like.  

For reviewing life and death situations occured in your real games.

For having fun playing contact fight if you are a beginner. Set a corner or a edge as the keypoint and then fight with the bot!

## Requirement
### Windows
- OS: 64-bit Windows  
- CPU: CPU with SSE 4.2 

## Author
AQ by [Yu Yamaguchi](https://twitter.com/ymg_aq)  
AQ-PS by Weizhi Nai  

