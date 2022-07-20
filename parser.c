#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct in {
	
	char			*txid;
	char			*vout;
	char			*script_sig;
	char			*sequence;
};

struct out {
	
	char			*amount;
	char			*pubkey;
};

struct segwit {

	char			*segwit_data;
	
};

struct transaction {
	
	char			version[9];
	int			segwit;		/* boolean */
	char			inputs[3];
	struct in		*in_arr;
	char			outputs[3];
	struct out		*out_arr;
	char			witness_cnt[3];
	struct segwit		*witness;
	char			locktime[9];
};


int main(int argc, char *argv[])
{
	int			i, j;
	int			num_inputs;
	char			*p, *buffer, *ptr;
	struct transaction	tx;

        
	if (argv[1] == NULL) {
		printf("usage: raw tx arg requrired");
		exit(1);
	}

	buffer = malloc(strlen(argv[1]));
	strcpy(buffer,argv[1]);
	ptr = buffer;

	/* **fix_me: will need to account for fd,fe,ff to expand this
	value if necessary.
	*/

	for (i = 0; i < 8; ++i) {
		tx.version[i] = *ptr++;
	}
	tx.version[i] = '\0';

	

        if (strncmp(ptr, "0001", 4) == 0) {
		ptr += 4;
		tx.segwit = 1;
	} else {
		tx.segwit = 0;
	}

	
        for (i = 0; i < 2; ++i) {
		tx.inputs[i] = *ptr++;
	}
	tx.inputs[i] = '\0';


	
	char *tmp = malloc(3 * sizeof(char));
	int  script_len;
	int  inp_cnt = strtoul(tx.inputs, &p, 16);
	tx.in_arr = malloc(inp_cnt * sizeof(struct in));

	/* inputs array routine  */
        for (i = 0; i < inp_cnt ; ++i) {

		tx.in_arr[i].txid = malloc(64);
		for (j = 0; j < 64; ++j) 
			tx.in_arr[i].txid[j] = *ptr++;
				
		tx.in_arr[i].vout = malloc(8);
                for (j = 0; j < 8; ++j) 
			tx.in_arr[i].vout[j] = *ptr++;

                for (j = 0; j < 2; ++j) {
			tmp[j] = *ptr++;
		}
		tmp[2] = '\0';

		script_len = strtoul(tmp, &p, 16);
		tx.in_arr[i].script_sig = malloc((2 * script_len + 1));

                for (j = 0; j < script_len * 2; ++j) 
			tx.in_arr[i].script_sig[j] = *ptr++;
		
		tx.in_arr[i].sequence = malloc(8);
                for (j = 0; j < 8; ++j) 
			tx.in_arr[i].sequence[j] = *ptr++;
	}


	for (i = 0; i < 2; ++i) {
		tx.outputs[i] = *ptr++;
	}
	tx.outputs[2] = '\0';
        int output_cnt = strtoul(tx.outputs, &p, 16);

	tx.out_arr = malloc(output_cnt * sizeof(struct out));

        /* outputs array routine */
        for (i = 0; i < output_cnt; ++i) {

		tx.out_arr[i].amount = malloc(16);
                for (j = 0; j < 16; ++j) 
			tx.out_arr[i].amount[j] = *ptr++;

                for (j = 0; j < 2; ++j) {
			tmp[j] = *ptr++;
		}
		tmp[2] = '\0';

		script_len = strtoul(tmp, &p, 16);
                tx.out_arr[i].pubkey = malloc((2 * script_len + 1));

                for (j = 0; j < script_len * 2; ++j) 
			tx.out_arr[i].pubkey[j] = *ptr++;
                
	}

			
	int wit_cnt = 0;

        if (tx.segwit) {

		for (i = 0; i < 2; ++i) {
			tx.witness_cnt[i] = *ptr++;
		}
		tx.witness_cnt[2] = '\0';

		wit_cnt = strtoul(tx.witness_cnt, &p, 16);
		tx.witness = malloc(wit_cnt * sizeof(struct segwit ));
		
		for (i = 0; i < wit_cnt; ++i) {
			for (j = 0; j < 2; ++j) {
				tmp[j] = *ptr++;
			}
                        tmp[2] = '\0';

			script_len = strtoul(tmp, &p, 16);
			tx.witness[i].segwit_data = malloc((2 * script_len + 1));

			for (j = 0; j < script_len * 2; ++j) 
				tx.witness[i].segwit_data[j] = *ptr++;
		}
	}
	
	for (i = 0; i < 8; ++i) {
		tx.locktime[i] = *ptr++;
	}
	tx.locktime[i] = '\0';
        
	printf("\n********************************************************************************\n");

        printf("\n\tverison:\t%s\n\n\t\tinput_cnt:\t%s",tx.version,tx.inputs);

        for (i = 0; i < inp_cnt ; ++i) {
		printf("\n__________________________________\n");
		printf("%d\n",i + 1);
		printf("txid:\t%s\n", tx.in_arr[i].txid);
		printf("vout:\t%s\n", tx.in_arr[i].vout);
	
		printf("sc_sig:\t%s\n", tx.in_arr[i].script_sig);
		printf("seq:\t%s\n", tx.in_arr[i].sequence);
        }

        printf("\n\t\toutput_cnt:\t%s",tx.outputs);

        for (i = 0; i < output_cnt ; ++i) {
		printf("\n__________________________________\n");
		printf("%d\n",i + 1);
                printf("amount:\t%s\n", tx.out_arr[i].amount);
		printf("pubkey:\t%s\n", tx.out_arr[i].pubkey);

        }

	if (tx.segwit) {

		printf("\n\t\twitness_cnt:\t%s",tx.witness_cnt);

                for (i = 0; i < wit_cnt; ++i) {
			printf("\n__________________________________\n");
			printf("%d\n",i + 1);
			printf("witness:\t%s\n", tx.witness[i].segwit_data);
		}
	}
	
	printf("\n\tlocktime:\t%s\n\n",tx.locktime);
	printf("\n********************************************************************************\n");
	return 0;
}



