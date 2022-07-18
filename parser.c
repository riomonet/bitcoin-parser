#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SEGWIT "0001"


struct inputs {
	
	char			txid[65];
	char			vin[9];
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
	struct inputs		*ins_arr;
	char			outputs[2];
	struct outputs		*outs_arr;
	char			locktime[8];
};


enum field_len{VERSION = 8, SWIT = 4, INPUTS = 2, OUTPUTS = 2, LOCKTIME = 8, TXID = 64, VIN = 8, SEQUENCE = 8};

/*
field(index,len)

     if segwit (8,4) == TRUE index += 4;

version		(0,8)
#inputs_basic	(8,2)
txid		(10,64)
vin		(74,8)
ss		(82,var1)
seq		(82+var1,8)
each inputlenght is  (80 + var1)

input 2 txid	(90  + var1, 64)
vin		(154 +var1, 8)
ss		(162+var1, var2)
seq		(162+var1+var2,8)

ouputs          (number of inputs *80 + var1,var2,var3...., 2)
amount		(outputs + 2, 16)

*/


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


	/* get the number if inputs this is going to be different for segwit,legacy, coinbase tx*/
	sub_str(tx.inputs, buffer, 8 + offset, INPUTS);
	num_inputs = strtoul(tx.inputs, &p, 16);




	
	/* check for segwit tx */
	sub_str(tx.seg_mkrflag, buffer, 8, SWIT);
        if(strcmp(tx.seg_mkrflag, SEGWIT) == 0)
		offset = 4;

	
	sub_str(tx.version, buffer, 0, VERSION);


	/* allocate input structs for each input */
	
	tx.ins_arr = malloc(num_inputs * sizeof(struct inputs));
	
	/* iterate over inputs */
	for (i = 0; i < num_inputs; i++) {

		sub_str(tx.ins_arr->txid, buffer, offset + 10, TXID);
		sub_str(tx.ins_arr->vin, buffer, offset + 74, VIN);
		sub_str(tmp, buffer, offset + 82, 2);
		ss_len = strtoul(tmp, &p, 16);
		tx.ins_arr->script_sig = malloc(2 * ss_len + 1);
		sub_str(tx.ins_arr->script_sig, buffer, offset + 84, ss_len * 2);
		sub_str(tx.ins_arr->sequence, buffer, offset + 84 + ss_len * 2, SEQUENCE);
		
	}

        
	
	/* size of a given input is txid + scriptsig + sequence
           calculate size for each input and that offset is where
           the next input begins or if there is none left
	   that is where the output begins.
	*/

	
        printf("\nversion\t%s\n",tx.version);
	printf("inputs\t%s\n",tx.inputs);
	printf("txid\t%s\n",tx.ins_arr->txid);
	printf("vin\t%s\n",tx.ins_arr->vin);
	printf("ssig\t%s\n",tx.ins_arr->script_sig);
	printf("seq\t%s\n",tx.ins_arr->sequence);
	return 0;
}


void sub_str(char *dest, char *src, int start_loc, int len)
{
	int j;

	for(j = 0 ; j < len; j++)
		dest[j] = src[start_loc++];
	dest[j] = '\0';
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

