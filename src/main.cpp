#include "main.h"


int main(int argc, char **argv) {

	ReadConfiguration(argc, argv);
	std::cerr << "configuration loaded.\n";

	// temp
	/*

	getchar();
	return 0;
	*/

	if(self_match)
		SelfMatch();
	else if (cfg_test_sgfs)	
		DoSgfs();
	else
		CallGTP();

	//DoSomething();

	std::cerr << "finished.\n";
	return 0;

}

void DoSgfs()
{
	std::cerr << "ko is " << (cfg_avoid_ko > 0 ? "avoided" : "allowed") << "\n";

	Board b;
	Tree tree;
	tree.InitBoard();
	std::vector<SgfData> sgf_list;
	ImportSGFList(resume_sgf_path, sgf_list);

	std::ofstream fo("output.txt");
	fo.close();
	std::string log_path = "log/0.txt";

	std::ofstream flog(log_path);
	flog << "ko is " << (cfg_avoid_ko > 0 ? "avoided" : "allowed") << "\n";
	flog.close();

	int fn = 0;
	double winrate = 0.5;
	for (SgfData sgf : sgf_list)
	{
		fn++;
		std::cerr << "\n\n" << sgf.filepath << "\n";
		tree.Clear();
		tree.log_path = log_path;

		sgf.GenerateBoard(b, sgf.move_cnt);
		tree.UpdateRootNode(b);

		int blackfirst = sgf.filepath.find("w") != std::string::npos ? 0 : 1;
		if (blackfirst != b.my)
		{
			b.PlayLegal(PASS);
			sgf.AddMove(PASS);
			--b.pass_cnt[b.her];
		}
		b.SelectKeypoint();

		std::cerr << "interested area is around " << CoordinateString(keypoint) << "\n";
		flog.open(log_path, std::ios::app);
		flog << "\n\n" << sgf.filepath << "\n";
		flog << "interested area is around " << CoordinateString(keypoint) << "\n";
		flog.close();
		
		winrate = 0.5;
		int next_move = tree.SearchTree(b, 0.0, winrate, true, false);
		b.PlayLegal(next_move);
		PrintBoard(b, next_move);

		fo.open("output.txt", std::ios::app);
		fo << sgf.filepath << "," << CoordinateString(next_move) << "\n";
		fo.close();


		PrintBoard(b, next_move, log_path);
	}
}

void ReadConfiguration(int argc, char **argv){

	ImportProbDist();
	ImportProbPtn3x3();

	for(int i=0;i<argc;++i){

		std::string str_arg = argv[i];
		std::string config_path = "aq_config.txt";
		if(str_arg.find("--config=") != std::string::npos){
			config_path = str_arg.substr(9);
			std::cerr << "configuration file path = " << config_path << std::endl;
		}

		// Open the configuration file.
		std::ifstream ifs(config_path);
		std::string str;

		// Read line by line.
		int line_cnt = 0;
		while (ifs && getline(ifs, str)) {

			if(str.find("=") != std::string::npos){
				auto head = str.find("=") + 1;
				str = str.substr(head);
				if(str.substr(0,1) == " ") str = str.substr(1);
			}
			else continue;		

			switch(line_cnt){
				case 0:  cfg_gpu_cnt 	= stoi(str); break;
				case 1:  cfg_thread_cnt 	= stoi(str); break;
				case 2:  cfg_main_time 	= stod(str); break;
				case 3:  cfg_byoyomi 	= stod(str); break;
				case 4:  need_time_controll = (str == "true" || str == "on"); break;
				case 5:  japanese_rule 	= (str == "true" || str == "on"); break;
				case 6:  cfg_komi 		= stod(str); break;
				case 7:  cfg_sym_idx 	= stoi(str); break;
				case 8:  cfg_mimic 		= (str == "true" || str == "on"); break;
				case 9:  never_resign 	= (str == "true" || str == "on"); break;
				case 10: self_match 	= (str == "true" || str == "on"); break;
				case 11: save_log 		= (str == "true" || str == "on"); break;
				case 12: is_master 		= (str == "true" || str == "on"); break;
				case 13: is_worker 		= (str == "true" || str == "on"); break;
				case 14: pb_dir 		= str; break;
				case 15: resume_sgf_path 	= str; break;
				case 16: cfg_worker_cnt 	= stoi(str); break;
				case 17: use_pondering 	= (str == "true" || str == "on"); break;
				case 18: cfg_custom_keypoint	= stoi(str); break;
				case 19: cfg_avoid_ko= stoi(str); break;
				case 20: cfg_test_sgfs		= (str == "true" || str == "on"); break;
				default: break;
			}
			++line_cnt;
		}
		ifs.close();
	}

}


void SelfMatch() {

	Tree tree;
	Board b;
	int next_move = PASS;
	int prev_move = VNULL;
	double win_rate = 0.5;
#ifdef _WIN32
	std::string log_path = "log\\0.txt";
#else
	std::string log_path = "log/0.txt";
#endif
	tree.log_path = log_path;

	for(int i=0;i<1;++i){
		std::ofstream ofs(log_path);
		ofs.close();

		b.Clear();
		prev_move = VNULL;
		win_rate = 0.5;

		while (b.move_cnt<720) {
			std::cerr << b.move_cnt << "\n";
			b.SelectKeypoint();
			next_move = tree.SearchTree(b, 0.0, win_rate, true, false);
			b.PlayLegal(next_move);
			PrintBoard(b, next_move);
			PrintBoard(b, next_move, log_path);
			if (next_move==PASS && prev_move==PASS) break;
			prev_move = next_move;
			//getchar();
		}

		tree.PrintResult(b);
		getchar();
	}

}
