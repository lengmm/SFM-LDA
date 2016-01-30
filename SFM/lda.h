#pragma once
#include<string>
#include<vector>
using std::string;
using std::vector;
class lda
{
public:
	lda();
	~lda();

	int init_est(double alpha, double beta, int topics, int iters, vector<vector<int> >& freq);//最后一个参数是词频数据
	// estimate LDA model using Gibbs sampling
	void estimate();
	int sampling(int m, int n);
	void compute_theta();
	void compute_phi();

	// init for inference
	int init_inf();
	// inference for new (unseen) data based on the estimated LDA model
	void inference();
	int inf_sampling(int m, int n);
	void compute_newtheta();
	void compute_newphi();


	// save LDA model to files
	// model_name.tassign: topic assignments for words in docs
	// model_name.theta: document-topic distributions
	// model_name.phi: topic-word distributions
	// model_name.others: containing other parameters of the model (alpha, beta, M, V, K)
	int lda::load_model(string model_name);
	int save_model(string model_name);
	int save_model_tassign(string filename);
	int save_model_theta(string filename);
	int save_model_phi(string filename);
	int save_model_others(string filename);
	int save_model_twords(string filename);

	// saving inference outputs
	int save_inf_model(string model_name);
	int save_inf_model_tassign(string filename);
	int save_inf_model_newtheta(string filename);
	int save_inf_model_newphi(string filename);
	int save_inf_model_others(string filename);
	int save_inf_model_twords(string filename);
private:

	int M; // dataset size (i.e., number of docs)
	int V; // vocabulary size
	int K; // number of topics
	int N; //每篇文章词的数量
	double alpha, beta; // LDA hyperparameters 
	int niters; // number of Gibbs sampling iterations

	int ** z; // topic assignments for words, size M x doc.size()
	int ** nw; // cwt[i][j]: number of instances of word/term i assigned to topic j, size V x K
	int ** nd; // na[i][j]: number of words in document i assigned to topic j, size M x K
	int * nwsum; // nwsum[j]: total number of words assigned to topic j, size K
	int * ndsum; // nasum[i]: total number of words in document i, size M
	double ** theta; // theta: document-topic distributions, size M x K
	double ** phi; // phi: topic-word distributions, size K x V
	string dir;			// model directory
	vector<vector<int> > frequence;

};

