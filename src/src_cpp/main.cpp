#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <ctime>
#include <chrono>

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

mat<bool> generate_mask(double (p), int n, int m) {
    int r1;

    mat<bool> mask(n,m,true);

    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    bernoulli_distribution dis(p);
    for (int a = 0; a < n; a++) {
        for (int b = 0; b < m; b++) {
            r1 = dis(gen);
            if (r1 == 1) {
                mask(a,b) = 0;
            }
        }
    }
    return mask;
}

mat<bool> generate_mask_from(mat<bool> mask, double p, int n, int m) {
    int r1;

    mat<bool> new_mask(n,m,true);
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution dis(p);

    for (int a = 0; a < n; a++) {
        for (int b = 0; b < m; b++) {
            new_mask(a,b) = mask(a,b);
            if (mask(a,b) == 0) {
                r1 = dis(gen);
                if (r1 == 1) {
                    new_mask(a,b) = 1;
                }
            }
        }
    }
    return new_mask;
}

mat<bool> vv(int t) {
    // (8,2) (8,8) (8,12) (13,6)
    mat<bool> errors(t,t,0);

    errors(13,5) = 1;

    return errors;
}

mat<bool> cc(int t) {
//     Matrix indices:
// (4,11) (6,0) (6,1) (10,5) (10,7)
    mat<bool> errors(t,t,0);

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
    vector<string> id_vector = {};
    // vector<string> id_vector = {"swap_1a33b842", "swap_6822f6e8","swap_adc9817d"};
    // vector<string> id_vector = {"swap_84f2e5cd"};
    // vector<string> id_vector = {"swac_7sxu71qo", "swac_gahdc3t2", "swac_4pi117ny"};
    // vector<string> id_vector = {"swac_4pi117ny"};
    // vector<string> id_vector = {"swac_3bs3ixvl", "swac_7sxu71qo", "swac_gahdc3t2"};
    // vector<string> id_vector = {"swac_4pi117ny"};
    // vector<string> id_vector = {"swac_zxe5e53k", "swac_aa0e3did", "swac_3bs3ixvl", "swac_7sxu71qo", "swac_gahdc3t2","swac_4pi117ny"};
    // vector<string> id_vector = {"swap_12f5a4a2", "swap_82698d8d", "swap_2e55c0b2", "swap_54d39ebd"};
    // vector<string> id_vector = {"swap_07d96dd9"};


    // Probabilities we want to run
    // vector<double> p {0.01};
    vector<double> proba_vector;
    for (int k = 1; k < 2; k+=2) {
            proba_vector.push_back(k*0.001);
    }
    vector<double> mask_vector;
    for (int k = 0; k < 1; k++) {
       mask_vector.push_back(k*0.1);
    }
    vector<int> time_vector;
    for (int k = 100; k < 1001; k+=100) {
        time_vector.push_back(k);
    }


    int no_test = 300000;
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////
    /////////////////////////////////////////


    // int algo = compute_algo();
    // char const * defaut_code_file_name = DEFAUT_CODE_FILE_NAME;


    ////////// Parser //////////
    // int c;
    double delay_saving = 0;
    int rand_seed = 2;

    // while((c = getopt(argc, argv, "o:t:n:"))!= -1){
    //     switch(c) {
    //         case 'n':
    //             if(optarg) rand_seed = stoi(optarg);
    //             break;
    //         case 'o':
    //             if(optarg) res_file_name = string(optarg);
    //             break;
    //         case 't':
    //             if(optarg) delay_saving = stoi(optarg);
                // case 'i':
                //     code_file_name_array = &(argv[optind]);
                //     no_code_file = 0;
                //     while(optind < argc){
                //      if(argv[optind][0] != '-'){
                //          optind++;
                //          no_code_file++;
                //      }
                //      else {
                //          break;
                //      }
                //     }
                // break;
    //     }
    // }

    if (optind == argc) {
            throw invalid_argument("No input file given");
    }
    int no_code_file = argc - optind;
    char const * const * code_file_name_array = &(argv[optind]);
    ////////////////////////////

    int seed = time(NULL) + 123 * rand_seed;
    srand (seed);
    string res_file_name = string("../results/cpp_laptop_") + generate_id() + ".res";
    string size_file_name = string("../results/size_") + generate_id() + ".txt";
    // string res_file_name = string("../results/cpp_laptop_") + "test.res";



    cout << "Output file: " << res_file_name << endl;
    cout << "seed: " << seed << endl;

    qcode_ensemble Q_ens(code_file_name_array, no_code_file, n_vector,
                         m_vector, dv_vector, dc_vector, id_vector);
    int no_codes = Q_ens.get_no_qcodes();
    cout << "no_codes = " << no_codes << endl;
    qcode* Q_ptr = Q_ens.get_qcode_ptr(0);


    ///////////////////////////////////////
    // To store the results
    Result_ensemble res_ens;

    ifstream file(res_file_name);
    if (file.fail())
        res_ens.to_file(res_file_name);
    else
        file.close();

    time_t last_saving;
    time(&last_saving);
    res_ens.add_file(res_file_name);
    auto start = chrono::high_resolution_clock::now();
    ///////////////////////////////////////


    // ISD

    for (int r = 0; r < no_test; r++) {
        for (int p_ind = 0; p_ind < int(proba_vector.size()); p_ind++) {
            double p = proba_vector[p_ind];
            for (int mask_ind = 0; mask_ind < int(mask_vector.size()); mask_ind++) {
                double p_mask = mask_vector[mask_ind];
                mat<bool> synd_mask = generate_mask(p_mask, Q_ptr->n, Q_ptr->m); // for everything but multiples of 100, ...
                mat<bool> partial_synd_mask = generate_mask_from(synd_mask, 0.9, Q_ptr->n, Q_ptr->m); // for multiples of 100
                mat<bool> partial_synd_mask2 = generate_mask_from(partial_synd_mask, 0.9, Q_ptr->n, Q_ptr->m);
                // multiples of 1000 ....

                for (int t_i = 0; t_i < int(time_vector.size()); t_i++) {
                    int t_ind = time_vector[t_i];
                    Decoder dec(Q_ptr->n,Q_ptr->m,Q_ptr->dv,Q_ptr->dc, Q_ptr->check_nbhd_ptr, Q_ptr->bit_nbhd_ptr);

                    mat<bool> vv_errors(Q_ptr->n, Q_ptr->n, 0);
                    mat<bool> cc_errors(Q_ptr->m, Q_ptr->m, 0);

                    // begin partial syndrome decoding
                    for (int t = 1; t < t_ind+1; t++) {
                        Decoder tmp_dec(Q_ptr->n,Q_ptr->m,Q_ptr->dv,Q_ptr->dc, Q_ptr->check_nbhd_ptr, Q_ptr->bit_nbhd_ptr);

                        vv_errors ^= generate_errors(p, Q_ptr->n);
                        cc_errors ^= generate_errors(p, Q_ptr->m);

    //                    if ((t % 1000) == 0) {
    //                        tmp_dec.decode(vv_errors, cc_errors, partial_synd_mask2);
    //                    } else if ((t % 100) == 0) {
    //                        tmp_dec.decode(vv_errors, cc_errors, partial_synd_mask);
    //                    } else {
    //                        tmp_dec.decode(vv_errors, cc_errors, synd_mask);
    //                    }

                        tmp_dec.decode(vv_errors, cc_errors, synd_mask);

                        vv_errors ^= tmp_dec.get_vv_correction();
                        cc_errors ^= tmp_dec.get_cc_correction();
                    }

                    vv_errors ^= generate_errors(p, Q_ptr->n);
                    cc_errors ^= generate_errors(p, Q_ptr->m);

                    dec.decode(vv_errors, cc_errors, generate_mask(0, Q_ptr->n, Q_ptr->m));

                    vv_errors ^= dec.get_vv_correction();
                    cc_errors ^= dec.get_cc_correction();

                    if (dec.get_synd_weight() != 0) {
                        res_ens.add_result(t_ind, Q_ptr->dv, Q_ptr->dc, Q_ptr->n, Q_ptr->m, Q_ptr->id, p, p_mask, 1, 0, 1);
                    } else {
                        int success = Q_ptr->is_stabilizer(vv_errors,cc_errors);
                        res_ens.add_result(t_ind, Q_ptr->dv, Q_ptr->dc, Q_ptr->n, Q_ptr->m, Q_ptr->id, p, p_mask, 1, success, 0);
                    }
                    res_ens.to_file(res_file_name);
                }
            }
        }
    }


    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << duration.count() << endl;
}
