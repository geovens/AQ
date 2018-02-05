#include "main.h"


int main(int argc, char **argv) {

	ReadConfiguration(argc, argv);
	CallGTP();
//	DoSomething();

	std::cerr << "finished.\n";
	return 0;

}

void ReadConfiguration(int argc, char **argv){

	// Get working directory path.
	char buf[1024] = {};
#ifdef _WIN32
	GetModuleFileName(NULL, buf, sizeof(buf)-1);
	std::string split_str = "\\";
#else
	readlink("/proc/self/exe", buf, sizeof(buf)-1);
	std::string split_str = "/";
#endif
	std::string path_(buf);
	// Delete file name.
	auto pos_filename = path_.rfind(split_str);
	if(pos_filename != std::string::npos){
		path_ = path_.substr(0, pos_filename + 1);
		// Use current directory if there is no configure file.
		std::ifstream ifs(path_ + "aq_config.txt");
		if(ifs.is_open()){
			working_dir = path_;
		}
	}


	ImportProbDist();
	ImportProbPtn3x3();
	std::string config_path = working_dir + "aq_config.txt";

	for(int i=0;i<argc;++i){

		std::string str_arg = argv[i];
		if(str_arg.find("--config=") != std::string::npos){
			config_path = str_arg.substr(9);
			std::cerr << "configuration file path = " << config_path << std::endl;
		}
		else if(str_arg.find("--make_learn") != std::string::npos){
			MakeLearningData();
		}

	}

	// Open the configuration file.
	std::ifstream ifs(config_path);
	std::string str;

	// Read line by line.
	int line_cnt = 0;
	while (ifs && getline(ifs, str)) {

		++line_cnt;
		// Exclude comment.
		auto cmt_pos = str.find("#");
		if(cmt_pos != std::string::npos){
			str = str.substr(0, cmt_pos);
		}
		str = TrimString(str, " \t\v\r\n-");
		if(str.length() == 0) continue;

		// Find position after '='.
		auto eql_pos = str.find("=") + 1;
		if(eql_pos == std::string::npos){
			std::cerr 	<< "Failed to parse config:" << config_path << ":"
						<< line_cnt << " " << str << ". '=' not found.\n";
			continue;
		}

		auto key = TrimString(str.substr(0, eql_pos - 1));
		auto val = TrimString(str.substr(eql_pos));

		if(key == "gpu count")	cfg_gpu_cnt = stoi(val);
		else if(key == "thread count") cfg_thread_cnt = stoi(val);
		else if(key == "main time[sec]") cfg_main_time = stod(val);
		else if(key == "byoyomi[sec]") cfg_byoyomi = stod(val);
		else if(key == "emergency time[sec]") cfg_emer_time = stod(val);
		else if(key == "time control") need_time_control = IsFlagOn(val);
		else if(key == "japanese rule") japanese_rule = IsFlagOn(val);
		else if(key == "komi") cfg_komi = stod(val);
		else if(key == "symmetrical index") cfg_sym_idx = stoi(val);
		else if(key == "mimic go") cfg_mimic = IsFlagOn(val);
		else if(key == "never resign") never_resign = IsFlagOn(val);
		else if(key == "self match") self_match = IsFlagOn(val);
		else if(key == "save log") save_log = IsFlagOn(val);
		else if(key == "master") is_master = IsFlagOn(val);
		else if(key == "worker") is_worker = IsFlagOn(val);
		else if(key == "worker count") cfg_worker_cnt = stoi(val);
		else if(key == "pb path") pb_dir = val;
		else if(key == "resume sgf path") resume_sgf_path = val;
		else if(key == "use pondering") use_pondering = IsFlagOn(val);
		else{
			std::cerr << "Unknown key: [" << key << "]" << std::endl;
		}
	}
	ifs.close();

	std::cerr << "configuration loaded.\n";
	//if(self_match) SelfMatch();
	if (self_match) DoSgfs();
}


void SelfMatch() {

	Tree tree;
	Board b;
	int next_move = PASS;
	int prev_move = VNULL;
	double win_rate = 0.5;
#ifdef _WIN32
	std::string log_path = working_dir + "log\\0.txt";
#else
	std::string log_path = working_dir + "log/0.txt";
#endif
	tree.log_file = new std::ofstream(log_path, std::ofstream::out);

	for(int i=0;i<1;++i){

		b.Clear();
		prev_move = VNULL;
		win_rate = 0.5;

		while (b.move_cnt<720) {
			next_move = tree.SearchTree(b, 0.0, win_rate, true, false);
			b.PlayLegal(next_move);
			PrintBoard(b, next_move);
			PrintBoard(b, next_move, tree.log_file);
			if (next_move==PASS && prev_move==PASS) break;
			prev_move = next_move;
		}

		tree.PrintResult(b);
	}

	std::cerr << "finished.\n";
	exit(0);

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
		tree.log_file = &flog;

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


		PrintBoard(b, next_move, tree.log_file);
	}
}