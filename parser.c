#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SEGWIT "0001"

struct in {
	
	char			txid[65];
	char			vout[9];
	char			*script_sig;
	char			sequence[8];
};

struct out {
	
	char			amount[17];
	char			*pubkey;

};

struct transaction {
	
	char			version[9];
	char			seg_mkrflag[5];
	char			inputs[3];
	struct in		*in_arr;
	char			outputs[2];
	struct out		*out_arr;
	char			locktime[9];
};


void sub_str(char *, char *, int, int);

int main(int argc, char *argv[])
{
	int			i, j;
	int			num_inputs, ss_len;
	char			c, *p, buffer[1000];
	struct transaction	tx;

        /* raw transaction is passed in thru argv[1],  I copy it to an char
           array called buffer, **fix_me: buffer should actually be delcared as
	   a char * instead of char arraay[1000] and I should Malloc()len of argv[1]).
	*/

	if (argv[1] == NULL) {
		printf("usage: raw tx arg requrired");
		exit(1);
	}
	
	strcpy(buffer,argv[1]);
	char *ptr = buffer;

        /* find script sig lengths for each input:
	the string starting at buffer plus 8 is the string representation of
        number of inputs, I need this value to calculate where the var_len
        fields are for each input. Need to convert it to an integer to make
        it useful. **fix_me: will need to account for fd,fe,ff to expand this
	value if necessary.
	*/

	
	for (i = 0; i < 8; ++i) {
		tx.version[i] = *ptr++;
	}
	tx.version[i] = '\0';

	for (i = 0; i < 2; ++i) {
		tx.inputs[i] = *ptr++;
	}
	tx.inputs[i] = '\0';

	char *tmp = malloc(3 * sizeof(char));
	int  script_len;
	int  inp_cnt = strtoul(tx.inputs, &p, 16);
	tx.in_arr = malloc(inp_cnt * sizeof(struct in));
	
        for (i = 0; i < inp_cnt ; ++i) {
		for (j = 0; j < 64; ++j) {
			tx.in_arr[i].txid[j] = *ptr++;
		}
		tx.in_arr[i].txid[64] = '\0';

		for (j = 0; j < 8; ++j) {
			tx.in_arr[i].vout[j] = *ptr++;
		}
		tx.in_arr[i].vout[8] = '\0';
		
                for (j = 0; j < 2; ++j) {
			tmp[j] = *ptr++;
		}
		tmp[2] = '\0';

		script_len = strtoul(tmp, &p, 16);
		tx.in_arr[i].script_sig = malloc(2 * script_len * sizeof(char));
		for (j = 0; j < script_len * 2; ++j) {
			tx.in_arr[i].script_sig[j] = *ptr++;
		}
		tx.in_arr[i].script_sig[script_len * 2] = '\0';
		for (j = 0; j < 8; ++j) {
			tx.in_arr[i].sequence[j] = *ptr++;
		}
		tx.in_arr[i].sequence[j] = '\0';
		
	}


	printf("%d\n", inp_cnt );

	printf("\n%s\n%s\n",tx.version,tx.inputs);
	for (i = 0; i < inp_cnt ; ++i) {
		printf("%s\n", tx.in_arr[i].txid);
		printf("%s\n", tx.in_arr[i].vout);
		printf("%s\n", tx.in_arr[i].script_sig);
		printf("%s\n", tx.in_arr[i].sequence);
	}

	return 0;
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


/*
field(index,len)

     if segwit (8,4) == TRUE index += 4;

version		(0,8)
#inputs_basic	(8,2)
txid		(10,64)
vin		(74,8)
ss		(82,var1)
seq		(82+var1,8)
EACH INPUTLENGHT IS  (80 + var1)

input 2 txid	(90  + var1, 64)
vin		(154 +var1, 8)
pss		(162+var1, var2)
seq		(162+var1+var2,8)

ouputs          (number of inputs *80 + var1,var2,var3...., 2)
amount		(outputs + 2, 16)

*/

/*
1. get number of inputs
2. where is the varlen field of the script sig for input 1
3. where is it for subsequent inputs ?


*/



/* 0100000006a26a1d036e1c65360ceb9ce77b1da6c6d39500aef6524a78286c166926a81d91000000008b483045022100ca361c8365289eb1cd27a939739242bd5d0bb13834b1597e3c835dbf9441ecee02204a51b0423a584d5bb53c210be3c0b798e9a44f5134d56807cf03654de99e917c014104c2845061cde5d61f135c7cc9b427c6cc8baa894d70a50e5a69f355daacf8f804780d3d76418af4bc72594767f2f324c18de991a9a9502f178db63c0ab261ee41ffffffff817f20eeb81773b3aa59390d8650e4d78749dde00703693fd0750e502ba221f1000000008c493046022100ec9642748fe1ce0fca6635afb2c7fc5d0f82e22e837643bce57ad9d367b538a0022100c9f4fa45e5515efad5152af687740269f723c1170b4b9142b76c8d3fb977c5ef014104d0f99cf68f130d7e12b39a0e022b71823b22d449872831b0315144dd50de37c45572694fda10fce9ff2c1f353384542b752888978c994a8fd5149f079c9a19adffffffff5b5c16ea7775a63c576e903672c0356c1c6fb7671649382780e5eba2218d7891010000008b483045022100e07d87f3c0a0abe3d887b74668109299ca7054136764658a2970bbe536a6f92d02207e151e3a9136f0d9b2789a4c83fbd0b239cdb18d3decbb217ecf5ccd8afffe460141047c0c4d119b9f0df71773c455207e770de97e1e052138a8fbb04aea9340aaea700fdef88cf1294cde90a446734eb490424727b0661c0ca7d2d5934d8b086bd70dffffffff787eee4dc013c8d0454ee3e5329030aee1481ca0164cff97b0ab854f89de09a5010000008b4830450220640ed72db0c6c427c0036a32a5ef751449ecf30a7c1ad276acc5b1b676600486022100e001c2391c9a5d5f8179675c84c18ea668b830b1ce68f476d506a06663055dd1014104a8747e8a29837f2b18d8e56c165c54b2e64ac90637bd1730fd68c97f1edac889ab84b7fc62a35a57322f5ef037c0f88d26ce811dc88c7ffc00a70f9994e71defffffffff1b667da92fec9e141b527e65e389439ee8f911f807fecb9b3e4493785b9a8e53000000008a47304402207981c209094335daf272e25877ecb19b38e02019a3d76590a55d6a3b621f4e96022062b3780c2807e7459bed1be86236360ac5c19fdbc5429bc371f2c872da84c43f0141041d04a4e938807cea2313d712daf49d600e164f7536f54665e5f7f55fdfdafc1b9db0bf4b7f7c7988ef61f3ff2f22e429742284cce94b76c98594dd3a874fbb05ffffffff33e1f60d3c1965b0e2f72fad712bacdf6650cb1f1e3c54563ebf72f862c3ae26010000008a47304402206d3866b2b00e18449ba1d9436972a49a55116c5ffa5a9ca48eb630a47e56eac6022063c72d5c1d4958a4368193c31d263e147f011b6bfea6669d4952bc01122f950e014104b3b8f4b793a6d46f3d8007ff65d3002a375057564d5eb7afe1e10a3db133fbbf3a182a354911ae00b999394e31d5d4cd9321b68bea48f6c41209d5d3db718798ffffffff0240420f00000000001976a914273d05143f352d9184838f59e93ceee09579179f88ac00c51c11030000001976a914bb01b691aeadceabc886c1ff1981ebaf9f6c692b88ac00000000
 */


/* 01000000
06
a26a1d036e1c65360ceb9ce77b1da6c6d39500aef6524a78286c166926a81d91
00000000
8b
483045022100ca361c8365289eb1cd27a939739242bd5d0bb13834b1597e3c835dbf9441ecee02204a51b0423a584d5bb53c210be3c0b798e9a44f5134d56807cf03654de99e917c014104c2845061cde5d61f135c7cc9b427c6cc8baa894d70a50e5a69f355daacf8f804780d3d76418af4bc72594767f2f324c18de991a9a9502f178db63c0ab261ee41
ffffffff
817f20eeb81773b3aa59390d8650e4d78749dde00703693fd0750e502ba221f1
00000000
8c
493046022100ec9642748fe1ce0fca6635afb2c7fc5d0f82e22e837643bce57ad9d367b538a0022100c9f4fa45e5515efad5152af687740269f723c1170b4b9142b76c8d3fb977c5ef014104d0f99cf68f130d7e12b39a0e022b71823b22d449872831b0315144dd50de37c45572694fda10fce9ff2c1f353384542b752888978c994a8fd5149f079c9a19ad
ffffffff
5b5c16ea7775a63c576e903672c0356c1c6fb7671649382780e5eba2218d7891
01000000
8b
483045022100e07d87f3c0a0abe3d887b74668109299ca7054136764658a2970bbe536a6f92d02207e151e3a9136f0d9b2789a4c83fbd0b239cdb18d3decbb217ecf5ccd8afffe460141047c0c4d119b9f0df71773c455207e770de97e1e052138a8fbb04aea9340aaea700fdef88cf1294cde90a446734eb490424727b0661c0ca7d2d5934d8b086bd70d
ffffffff
787eee4dc013c8d0454ee3e5329030aee1481ca0164cff97b0ab854f89de09a5
01000000
8b
4830450220640ed72db0c6c427c0036a32a5ef751449ecf30a7c1ad276acc5b1b676600486022100e001c2391c9a5d5f8179675c84c18ea668b830b1ce68f476d506a06663055dd1014104a8747e8a29837f2b18d8e56c165c54b2e64ac90637bd1730fd68c97f1edac889ab84b7fc62a35a57322f5ef037c0f88d26ce811dc88c7ffc00a70f9994e71de
fffffffff
1b667da92fec9e141b527e65e389439ee8f911f807fecb9b3e4493785b9a8e53
00000000
8a
47304402207981c209094335daf272e25877ecb19b38e02019a3d76590a55d6a3b621f4e96022062b3780c2807e7459bed1be86236360ac5c19fdbc5429bc371f2c872da84c43f0141041d04a4e938807cea2313d712daf49d600e164f7536f54665e5f7f55fdfdafc1b9db0bf4b7f7c7988ef61f3ff2f22e429742284cce94b76c98594dd3a874fbb05
ffffffff
33e1f60d3c1965b0e2f72fad712bacdf6650cb1f1e3c54563ebf72f862c3ae26010000008a47304402206d3866b2b00e18449ba1d9436972a49a55116c5ffa5a9ca48eb630a47e56eac6022063c72d5c1d4958a4368193c31d263e147f011b6bfea6669d4952bc01122f950e014104b3b8f4b793a6d46f3d8007ff65d3002a375057564d5eb7afe1e10a3db133fbbf3a182a354911ae00b999394e31d5d4cd9321b68bea48f6c41209d5d3db718798ffffffff0240420f00000000001976a914273d05143f352d9184838f59e93ceee09579179f88ac00c51c11030000001976a914bb01b691aeadceabc886c1ff1981ebaf9f6c692b88ac00000000 */
