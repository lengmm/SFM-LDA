#include "lda.h"
#include<ctime>
#include "Config.h"
#define	BUFF_SIZE_LONG	1000000
extern Config cf;
lda::lda()
{

}
lda::~lda()
{
	if (z) {
		for (int m = 0; m < M; m++) {
			if (z[m]) {
				delete z[m];
			}
		}
	}

	if (nw) {
		for (int w = 0; w < V; w++) {
			if (nw[w]) {
				delete nw[w];
			}
		}
	}

	if (nd) {
		for (int m = 0; m < M; m++) {
			if (nd[m]) {
				delete nd[m];
			}
		}
	}

	if (nwsum) {
		delete nwsum;
	}

	if (ndsum) {
		delete ndsum;
	}

	if (theta) {
		for (int m = 0; m < M; m++) {
			if (theta[m]) {
				delete theta[m];
			}
		}
	}

	if (phi) {
		for (int k = 0; k < K; k++) {
			if (phi[k]) {
				delete phi[k];
			}
		}
	}
}
int lda::init_est(double alpha, double beta, int topics,int iters, vector<vector<int> >& freq){
	this->alpha = alpha;
	this->beta = beta;
	K = topics;
	niters = iters;
	frequence = freq;
	dir = "./result/model/";
	M = freq.end() - freq.begin();
	auto beg = freq.begin();
	V = beg->end() - beg->begin();
	nw = new int*[V];
	for (int w = 0; w < V; w++) {
		nw[w] = new int[K];
		for (int k = 0; k < K; k++) {
			nw[w][k] = 0;
		}
	}
	nd = new int*[M];
	for (int m = 0; m < M; m++) {
		nd[m] = new int[K];
		for (int k = 0; k < K; k++) {
			nd[m][k] = 0;
		}
	}
	nwsum = new int[K];
	for (int k = 0; k < K; k++) {
		nwsum[k] = 0;
	}

	ndsum = new int[M];
	for (int m = 0; m < M; m++) {
		ndsum[m] = 0;
	}

	srand(time(0)); // initialize for random number generation
	z = new int*[M];
	for (int m = 0; m <M; m++) {
		auto docbeg =freq.begin() + m;
		int N = 0;
		//根据词频文件统计出每篇文章单词的个数
		for (auto b = docbeg->begin(); b != docbeg->end(); b++)
			N += *b;
		z[m] = new int[N];
		// initialize for z
		for (int n = 0; n < N; n++) {
			int topic = (int)(((double)rand() / RAND_MAX) * K);
			z[m][n] = topic;
			// number of instances of word i assigned to topic j
			int wn=0;
			for (int w = 0; w < cf.centerCount; w++)
			{
				int sum = 0;
				if (sum<n&&sum + freq[m][w]>n){
					wn = w;
					break;
				}
				sum += freq[m][w];
			}
			nw[wn][topic] += 1;
			// number of words in document i assigned to topic j
			nd[m][topic] += 1;
			// total number of words assigned to topic j
			nwsum[topic] += 1;
		}
		// total number of words in document i
		ndsum[m] = N;
	}

	theta = new double*[M];
	for (int m = 0; m < M; m++) {
		theta[m] = new double[K];
	}

	phi = new double*[K];
	for (int k = 0; k < K; k++) {
		phi[k] = new double[V];
	}

	return 0;
}
// estimate LDA model using Gibbs sampling
void lda::estimate(){
	for (int liter = 1; liter <= niters; liter++) {
		printf("Iteration %d ...\n", liter);
		// for all z_i
		for (int m = 0; m < M; m++) {
			for (int n = 0; n < N; n++) {
				// (z_i = z[m][n])
				// sample from p(z_i|z_-i, w)
				int topic = sampling(m, n);
				z[m][n] = topic;
			}
		}
	
	}
	printf("Gibbs sampling completed!\n");
	printf("Saving the final model!\n");
	compute_theta();
	compute_phi();
	save_model(string("model"));

}
int lda::sampling(int m, int n){
	// remove z_i from the count variables
	int topic = z[m][n];
	int wn;
	for (int w = 0; w < cf.centerCount; w++)
	{
		int sum = 0;
		if (sum<n&&sum + frequence[m][w]>n){
			wn = w;
		}

	}
	int w = wn;
	nw[w][topic] -= 1;
	nd[m][topic] -= 1;
	nwsum[topic] -= 1;
	ndsum[m] -= 1;

	double Vbeta = V * beta;
	double Kalpha = K * alpha;
	// do multinomial sampling via cumulative method
	double * p; // temp variable for sampling
	p = new double[K];
	for (int k = 0; k < K; k++) {
		p[k] = (nw[w][k] + beta) / (nwsum[k] + Vbeta) *
			(nd[m][k] + alpha) / (ndsum[m] + Kalpha);
	}
	// cumulate multinomial parameters
	for (int k = 1; k < K; k++) {
		p[k] += p[k - 1];
	}
	// scaled sample because of unnormalized p[]
	double u = ((double)rand() / RAND_MAX) * p[K - 1];

	for (topic = 0; topic < K; topic++) {
		if (p[topic] > u) {
			break;
		}
	}

	// add newly estimated z_i to count variables
	nw[w][topic] += 1;
	nd[m][topic] += 1;
	nwsum[topic] += 1;
	ndsum[m] += 1;

	delete p;
	return topic;
}
void lda::compute_theta(){
	for (int m = 0; m < M; m++) {
		for (int k = 0; k < K; k++) {
			theta[m][k] = (nd[m][k] + alpha) / (ndsum[m] + K * alpha);
		}
	}
}
void lda::compute_phi(){
	for (int k = 0; k < K; k++) {
		for (int w = 0; w < V; w++) {
			phi[k][w] = (nw[w][k] + beta) / (nwsum[k] + V * beta);
		}
	}
}



int lda::save_model(string model_name) {
	if (save_model_tassign(dir + model_name + ".tassign")) {
		return 1;
	}

	if (save_model_others(dir + model_name + ".others")) {
		return 1;
	}

	if (save_model_theta(dir + model_name + ".theta")) {
		return 1;
	}

	if (save_model_phi(dir + model_name + ".phi")) {
		return 1;
	}

	return 0;
}
int lda::save_model_theta(string filename) {
	FILE * fout = fopen(filename.c_str(), "w");
	if (!fout) {
		printf("Cannot open file %s to save!\n", filename.c_str());
		return 1;
	}

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < K; j++) {
			fprintf(fout, "%f ", theta[i][j]);
		}
		fprintf(fout, "\n");
	}

	fclose(fout);

	return 0;
}
int lda::save_model_phi(string filename) {
	FILE * fout = fopen(filename.c_str(), "w");
	if (!fout) {
		printf("Cannot open file %s to save!\n", filename.c_str());
		return 1;
	}

	for (int i = 0; i < K; i++) {
		for (int j = 0; j < V; j++) {
			fprintf(fout, "%f ", phi[i][j]);
		}
		fprintf(fout, "\n");
	}

	fclose(fout);

	return 0;
}
int lda::save_model_others(string filename) {
	FILE * fout = fopen(filename.c_str(), "w");
	if (!fout) {
		printf("Cannot open file %s to save!\n", filename.c_str());
		return 1;
	}

	fprintf(fout, "alpha=%f\n", alpha);
	fprintf(fout, "beta=%f\n", beta);
	fprintf(fout, "ntopics=%d\n", K);
	fprintf(fout, "ndocs=%d\n", M);
	fprintf(fout, "nwords=%d\n", V);

	fclose(fout);

	return 0;
}
int lda::save_model_tassign(string filename) {
	int i, j;

	FILE * fout = fopen(filename.c_str(), "w");
	if (!fout) {
		printf("Cannot open file %s to save!\n", filename.c_str());
		return 1;
	}

	// wirte docs with topic assignments for words
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			fprintf(fout, "%d,%d:%d ", i,j, z[i][j]);
		}
		fprintf(fout, "\n");
	}

	fclose(fout);

	return 0;
}





//int lda::load_model(string model_name) {
//	int i, j;
//
//	string filename = dir + model_name + ".tassign";
//	FILE * fin = fopen(filename.c_str(), "r");
//	if (!fin) {
//		printf("Cannot open file %d to load model!\n", filename.c_str());
//		return 1;
//	}
//
//	char buff[BUFF_SIZE_LONG];
//	string line;
//
//	// allocate memory for z and ptrndata
//	z = new int*[M];
//	ptrndata = new dataset(M);
//	ptrndata->V = V;
//
//	for (i = 0; i < M; i++) {
//		char * pointer = fgets(buff, BUFF_SIZE_LONG, fin);
//		if (!pointer) {
//			printf("Invalid word-topic assignment file, check the number of docs!\n");
//			return 1;
//		}
//
//		line = buff;
//		strtokenizer strtok(line, " \t\r\n");
//		int length = strtok.count_tokens();
//
//		vector<int> words;
//		vector<int> topics;
//		for (j = 0; j < length; j++) {
//			string token = strtok.token(j);
//
//			strtokenizer tok(token, ":");
//			if (tok.count_tokens() != 2) {
//				printf("Invalid word-topic assignment line!\n");
//				return 1;
//			}
//
//			words.push_back(atoi(tok.token(0).c_str()));
//			topics.push_back(atoi(tok.token(1).c_str()));
//		}
//
//		// allocate and add new document to the corpus
//		document * pdoc = new document(words);
//		ptrndata->add_doc(pdoc, i);
//
//		// assign values for z
//		z[i] = new int[topics.size()];
//		for (j = 0; j < topics.size(); j++) {
//			z[i][j] = topics[j];
//		}
//	}
//
//	fclose(fin);
//
//	return 0;
//}

//// init for inference
//int lda::init_inf(){
//	return 0;
//}
//// inference for new (unseen) data based on the estimated LDA model
//void lda::inference(){
//
//}
//int lda::inf_sampling(int m, int n){
//
//	return 0;
//}
//void lda::compute_newtheta(){
//
//}
//void lda::compute_newphi(){
//
//}
//
//int lda::save_inf_model(string model_name) {
//	if (save_inf_model_tassign(dir + model_name + tassign_suffix)) {
//		return 1;
//	}
//
//	if (save_inf_model_others(dir + model_name + others_suffix)) {
//		return 1;
//	}
//
//	if (save_inf_model_newtheta(dir + model_name + theta_suffix)) {
//		return 1;
//	}
//
//	if (save_inf_model_newphi(dir + model_name + phi_suffix)) {
//		return 1;
//	}
//
//	if (twords > 0) {
//		if (save_inf_model_twords(dir + model_name + twords_suffix)) {
//			return 1;
//		}
//	}
//
//	return 0;
//}
//
//int lda::save_inf_model_tassign(string filename) {
//	int i, j;
//
//	FILE * fout = fopen(filename.c_str(), "w");
//	if (!fout) {
//		printf("Cannot open file %s to save!\n", filename.c_str());
//		return 1;
//	}
//
//	// wirte docs with topic assignments for words
//	for (i = 0; i < pnewdata->M; i++) {
//		for (j = 0; j < pnewdata->docs[i]->length; j++) {
//			fprintf(fout, "%d:%d ", pnewdata->docs[i]->words[j], newz[i][j]);
//		}
//		fprintf(fout, "\n");
//	}
//
//	fclose(fout);
//
//	return 0;
//}
//
//int lda::save_inf_model_newtheta(string filename) {
//	int i, j;
//
//	FILE * fout = fopen(filename.c_str(), "w");
//	if (!fout) {
//		printf("Cannot open file %s to save!\n", filename.c_str());
//		return 1;
//	}
//
//	for (i = 0; i < newM; i++) {
//		for (j = 0; j < K; j++) {
//			fprintf(fout, "%f ", newtheta[i][j]);
//		}
//		fprintf(fout, "\n");
//	}
//
//	fclose(fout);
//
//	return 0;
//}
//
//int lda::save_inf_model_newphi(string filename) {
//	FILE * fout = fopen(filename.c_str(), "w");
//	if (!fout) {
//		printf("Cannot open file %s to save!\n", filename.c_str());
//		return 1;
//	}
//
//	for (int i = 0; i < K; i++) {
//		for (int j = 0; j < newV; j++) {
//			fprintf(fout, "%f ", newphi[i][j]);
//		}
//		fprintf(fout, "\n");
//	}
//
//	fclose(fout);
//
//	return 0;
//}
//
//int lda::save_inf_model_others(string filename) {
//	FILE * fout = fopen(filename.c_str(), "w");
//	if (!fout) {
//		printf("Cannot open file %s to save!\n", filename.c_str());
//		return 1;
//	}
//
//	fprintf(fout, "alpha=%f\n", alpha);
//	fprintf(fout, "beta=%f\n", beta);
//	fprintf(fout, "ntopics=%d\n", K);
//	fprintf(fout, "ndocs=%d\n", newM);
//	fprintf(fout, "nwords=%d\n", newV);
//	fprintf(fout, "liter=%d\n", inf_liter);
//
//	fclose(fout);
//
//	return 0;
//}
//
//int lda::save_inf_model_twords(string filename) {
//	FILE * fout = fopen(filename.c_str(), "w");
//	if (!fout) {
//		printf("Cannot open file %s to save!\n", filename.c_str());
//		return 1;
//	}
//
//	if (twords > newV) {
//		twords = newV;
//	}
//	mapid2word::iterator it;
//	map<int, int>::iterator _it;
//
//	for (int k = 0; k < K; k++) {
//		vector<pair<int, double> > words_probs;
//		pair<int, double> word_prob;
//		for (int w = 0; w < newV; w++) {
//			word_prob.first = w;
//			word_prob.second = newphi[k][w];
//			words_probs.push_back(word_prob);
//		}
//
//		// quick sort to sort word-topic probability
//		utils::quicksort(words_probs, 0, words_probs.size() - 1);
//
//		fprintf(fout, "Topic %dth:\n", k);
//		for (int i = 0; i < twords; i++) {
//			_it = pnewdata->_id2id.find(words_probs[i].first);
//			if (_it == pnewdata->_id2id.end()) {
//				continue;
//			}
//			it = id2word.find(_it->second);
//			if (it != id2word.end()) {
//				fprintf(fout, "\t%s   %f\n", (it->second).c_str(), words_probs[i].second);
//			}
//		}
//	}
//
//	fclose(fout);
//
//	return 0;
//}
