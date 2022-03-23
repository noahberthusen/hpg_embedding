#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <ctime>

#include "qcode.h"
#include "decoder.h"
#include "write_result.h"
#include "generate_id.h"
#include "input_params.h"
#include "flip.h"

//To-do
//Put all compute_rref's in one function
//Have a reset button!
//Fix indentation!

mat<bool> generate_errors(double (p), int t) {
    int r1;

    mat<bool> errors(t,t,0);

    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    bernoulli_distribution dis(p);
    for (int a = 0; a < t; a++) {
	for (int b = 0; b < t; b++) {
	    r1 = dis(gen);
	    if (r1 == 1) {
		errors(a,b) = 1;
	    }
	}
    }

    return errors;
}

int compute_algo() {
#if (!flip_flag)
#if ((weight_flag) && (!sampling_flag))
    int ans = 1;
#elif ((!weight_flag) && (!sampling_flag))
    int ans = 2;
#elif ((weight_flag) && (sampling_flag))
    int ans = 3;
#else
    int ans = 4;
#endif
#else
#if ((weight_flag) && (!sampling_flag))
    int ans = 5;
#elif ((!weight_flag) && (!sampling_flag))
    int ans = 6;
#elif ((weight_flag) && (sampling_flag))
    int ans = 7;
#else
    int ans = 8;
#endif
#endif


    return ans;
}

int main(int argc, char * argv[]) {


    ////////////// Modify here //////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    // Use these varibles to select code: the simulation will run only for codes which match one parameter in each one of the folowing vectors.
    vector<int> n_vector {};
    vector<int> m_vector = {};
    vector<int> dv_vector = {};
    vector<int> dc_vector = {};
    // Use quotes !
    // vector<string> id_vector = {};
    // vector<string> id_vector = {"swap_1a33b842", "swap_6822f6e8","swap_adc9817d"};
    // vector<string> id_vector = {"swap_84f2e5cd"};
    // vector<string> id_vector = {"swac_7sxu71qo", "swac_gahdc3t2", "swac_4pi117ny"};
    // vector<string> id_vector = {"swac_4pi117ny"};
    // vector<string> id_vector = {"swac_3bs3ixvl", "swac_7sxu71qo", "swac_gahdc3t2"};
    vector<string> id_vector = {"swac_4pi117ny"};
    // vector<string> id_vector = {"swac_zxe5e53k", "swac_aa0e3did", "swac_3bs3ixvl", "swac_7sxu71qo", "swac_gahdc3t2","swac_4pi117ny"};
    // vector<string> id_vector = {"swap_12f5a4a2", "swap_82698d8d", "swap_2e55c0b2", "swap_54d39ebd"};
    // vector<string> id_vector = {"swap_07d96dd9"};

    
    // Probabilities we want to run
    // vector<double> p {0.01};
    vector<double> proba_vector;
    for (int k = 1; k < 2; ++k) {
	proba_vector.push_back(k*0.045);
    }
    
    int no_test = 1000000;
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////


    int algo = compute_algo();
    // char const * defaut_code_file_name = DEFAUT_CODE_FILE_NAME;
    string res_file_name = DEFAULT_RES_FILE_NAME;


    ////////// Parser //////////
    int c;
    double delay_saving = 0;
    int rand_seed = 0;
    
    while((c = getopt(argc, argv, "o:t:n:"))!= -1){
	switch(c) {
	case 'n':
	    if(optarg) rand_seed = stoi(optarg);
	    break;
	case 'o':
	    if(optarg) res_file_name = string(optarg);
	    break;
	case 't':
	    if(optarg) delay_saving = stoi(optarg);
	    // case 'i':
	    //     code_file_name_array = &(argv[optind]);
	    //     no_code_file = 0;
	    //     while(optind < argc){
	    // 	if(argv[optind][0] != '-'){
	    // 	    optind++;
	    // 	    no_code_file++;
	    // 	}
	    // 	else {
	    // 	    break;
	    // 	}
	    //     }
	    // break;	    
	}
    }

    if (optind == argc) {
	throw invalid_argument("No input file given");
    }
    int no_code_file = argc - optind;
    char const * const * code_file_name_array = &(argv[optind]);
    ////////////////////////////

    int seed = time(NULL) + 123 * rand_seed;
    srand (seed);

    
    cout << "Output file: " << res_file_name << endl;
    cout << "seed: " << seed << endl;
    
    qcode_ensemble Q_ens(code_file_name_array, no_code_file, n_vector,
			 m_vector, dv_vector, dc_vector, id_vector);
    int no_codes = Q_ens.get_no_qcodes();
    cout << "no_codes = " << no_codes << endl;

    
    ///////////////////////////////////////
    // To store the results
    Result_ensemble res_ens;
    
    ifstream file(res_file_name);
    if (file.fail())
	res_ens.to_file(res_file_name);
    else
	file.close();
    
    res_ens.add_file(res_file_name);
    time_t last_saving;
    time(&last_saving);
    ///////////////////////////////////////
	
    for (int r = 0; r < no_test; r++) {
	for (int proba_ind = 0; proba_ind < int(proba_vector.size()); ++ proba_ind) {
	    double p = proba_vector[proba_ind];
	    for (int code_ind = 0; code_ind < no_codes; code_ind++) {
		qcode* Q_ptr = Q_ens.get_qcode_ptr(code_ind);
				
		mat<bool> vv_errors = generate_errors(p, Q_ptr->n);
		mat<bool> cc_errors = generate_errors(p, Q_ptr->m);

#if flip_flag
		flip flp(Q_ptr->n,Q_ptr->m,Q_ptr->dv,Q_ptr->dc, Q_ptr->check_nbhd_ptr, Q_ptr->bit_nbhd_ptr);
		flp.compute_syndrome_matrix_ptr(vv_errors, cc_errors);
		flp.decode();

		vv_errors = *flp.vv_qbits_ptr ^ vv_errors;
		cc_errors = *flp.cc_qbits_ptr ^ cc_errors;
#endif

		Decoder dec(Q_ptr->n,Q_ptr->m,Q_ptr->dv,Q_ptr->dc, Q_ptr->check_nbhd_ptr, Q_ptr->bit_nbhd_ptr);
		dec.decode(vv_errors, cc_errors);

		mat<bool> final_vv_errors = dec.get_vv_correction() ^ vv_errors;
		mat<bool> final_cc_errors = dec.get_cc_correction() ^ cc_errors;

		if (dec.get_synd_weight() != 0) {
		    res_ens.add_result(algo, Q_ptr->dv, Q_ptr->dc, Q_ptr->n, Q_ptr->m, Q_ptr->id, p, 1, 0, 1);
		}
		else {
		    int success = Q_ptr->is_stabilizer(final_vv_errors,final_cc_errors);
		    res_ens.add_result(algo, Q_ptr->dv, Q_ptr->dc, Q_ptr->n, Q_ptr->m, Q_ptr->id, p, 1, success, 0);
		}
		if (difftime(time(NULL),last_saving) > delay_saving) {
		    time(&last_saving);
		    res_ens.to_file(res_file_name);
		}
	    }
	}
    }
}