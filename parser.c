#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SEGWIT "0001"


struct inputs {
	
	char			txid[64];
	char			vin[8];
	char			*script_sig;
	char			sequence[8];
	int			len;
};

struct outputs {
	
	char			txid[64];
	char			*script_sig;
	char			sequence[8];
};

struct transaction {
	
	char			version[8];
	char			seg_mkrflag[4];
	char			inputs[2];
	struct inputs		*inputs_array;
	char			outputs[2];
	struct outputs		*outputs_array;
	char			locktime[8];
};

enum field_len{version = 8, segwit = 4, inputs = 2, outputs = 2, locktime = 8, TXID = 64};

void sub_str(char *, char *, int, int);

int main(int argc, char *argv[])
{
	int			i, offset = 0;
	int			num_inputs, ss_len;
	char			c, *p, buffer[1000];
	struct transaction	tx;
	struct inputs		*inp;
	char			tmp[2];


	/* if there is no argument exit */
	if (argv[1] == NULL) {
		printf("usage: raw tx arg requrired");
		exit(1);
	}

	strcpy(buffer,argv[1]);
	
	/* check for segwit tx */
	sub_str(tx.seg_mkrflag, buffer, 8, segwit);
        if(strcmp(tx.seg_mkrflag, SEGWIT) == 0)
		offset = 4;

	
	sub_str(tx.version, buffer, 0, version);
        sub_str(tx.inputs, buffer, 8 + offset, inputs);

	/* allocate input structs for each input */
	num_inputs = strtoul(tx.inputs, &p, 16);
	inp = malloc(num_inputs * sizeof(struct inputs));

	/* iterate over inputs */
	offset += inp->len;
	for (i = 0; i < num_inputs; ++i) {
		sub_str(inp->txid, buffer, offset + 10, TXID);
		sub_str(inp->vin, buffer, offset + 74, 8);
		sub_str(tmp, buffer, offset + 82, 2);
		ss_len = strtoul(tmp, &p, 16);
		
	}

        tx.inputs_array = inp;
	
	/* size of a given input is txid + scriptsig + sequence
           calculate size for each input and that offset is where
           the next input begins or if there is none left
	   that is where the output begins.
	*/


        printf("\nversion\t%s\n",tx.version);
	printf("inputs\t%s\n",tx.inputs);
	printf("txid\t%s\n",tx.inputs_array[0].txid);
	printf("vin\t%s\n",tx.inputs_array[0].vin);
	printf("ss_len\t%d\n",ss_len );
	return 0;
}


void sub_str(char *dest, char *src, int start_loc, int len)
{
	int i;

	for(i = 0 ; i < len; i++)
		dest[i] = src[start_loc++];
	dest[i] = '\0';
}	


/* random raw transaction
version 01000000   4  bytes  8 characters     0-7
segwit markerand flag 2 bytes 4 characters    8-11
inputs 01          1 byte    2 characters     either 8-9 or 12-13
txid 38966caadb834f37ddb3c51ec417dea9cd9c392e3ab465edd7959de6652c6b4e   32 bytes
64 characters  either 10-73 or 14-78
vin 00000000  
8b
scriptsig, long script sig, parse it out.
483045022100a66c833777f44b828bc1c327e4ee59fde31fd72b5577efabd85090d4ff8f9b6e022041921d797c592df2d566cc7f4fa119ada1aa3e7c8145c9543dd1e5ba765434c20141042ef60f0a969878a7d6acb4e8ba2645771c25b9713ceb4b545678bdfcc0507ab8b7c6650579c0f1b8ac09998501d372aa93657829c4f3c4a8268f0cf87aa8784c
seq; ffffffff
outputs 02
amount 1e5f990200000000
pubkey 1976a9149e71046a35c496d7035e57b564f680adb9a13e9688ac
amount 7100700000000000
pubkey 1976a914197807be6fab6fa0ccc6f0f6047a63b398b309f088ac
locktime 00000000 */


/* same as above unserialized 010000000138966caadb834f37ddb3c51ec417dea9cd9c392e3ab465edd7959de6652c6b4e000000008b483045022100a66c833777f44b828bc1c327e4ee59fde31fd72b5577efabd85090d4ff8f9b6e022041921d797c592df2d566cc7f4fa119ada1aa3e7c8145c9543dd1e5ba765434c20141042ef60f0a969878a7d6acb4e8ba2645771c25b9713ceb4b545678bdfcc0507ab8b7c6650579c0f1b8ac09998501d372aa93657829c4f3c4a8268f0cf87aa8784cffffffff021e5f9902000000001976a9149e71046a35c496d7035e57b564f680adb9a13e9688ac71007000000000001976a914197807be6fab6fa0ccc6f0f6047a63b398b309f088ac00000000
*/
/* catch errors, make sure the inpute string is of a certain lenght and only has legal characters */
/* we can determine start and end of every string that needs to be parsed, maybe
that should be all calculated first and placed in multi dimesnional array or
some other data structure instead of figure out as you gom
we need to start off by calculating the length of all the varibale length fields
I will only do the first byte now, I will take care of fd fe ff later, for more
complex
parsing.
also need to account for coinbase tx's.

*/

