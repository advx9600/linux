# http://blog.sina.com.cn/s/blog_61c485330100nkys.html for matlab
# http://blog.sina.com.cn/s/blog_61c485330100in91.html yes no recog

DIRS="sig mfcc model/proto train/init"
TEMP_FILE="temp_save.txt"

for i in $DIRS
do
	if [ ! -e $i ];then
		mkdir $i -p
	fi
done

cat > extract.conf<<EOF
SOURCEFORMAT = HTK # Gives the format of the speech files
TARGETKIND = MFCC_D_A_0 # Identifier of the coefficients to use
# Unit = 0.1 micro-second :
WINDOWSIZE = 250000.0 # = 25 ms = length of a time frame
TARGETRATE = 100000.0 # = 10 ms = frame periodicity
NUMCEPS = 12 # Number of MFCC coeffs (here from c1 to c12)
USEHAMMING = T # Use of Hamming function for windowing frames
PREEMCOEF = 0.97 # Pre-emphasis coefficient
NUMCHANS = 26 # Number of filterbank channels
CEPLIFTER = 22 # Length of cepstral liftering
EOF

ls sig/*.sig | sort > $TEMP_FILE 

awk '{split($1,s,"/");split(s[2],h,".");
	print $1 "  mfcc/"h[1]".mfcc";}' $TEMP_FILE > iofile.txt

HCopy  -C extract.conf  -S iofile.txt

for  i in dilili sil
do
	cat > model/proto/hmm_${i}.hmm <<EOF
~o <VecSize> 39 <MFCC_D_A_0>
~h "${i}"
<BeginHMM>
<NumStates> 6
<State> 2
<Mean> 39
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
<Variance> 39
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
<State> 3
<Mean> 39
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
<Variance> 39
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
<State> 4
<Mean> 39
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
<Variance> 39
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
<State> 5
<Mean> 39
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
<Variance> 39
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
<TransP> 6
0.0 0.5 0.5 0.0 0.0 0.0
0.0 0.4 0.3 0.3 0.0 0.0
0.0 0.0 0.4 0.3 0.3 0.0
0.0 0.0 0.0 0.4 0.3 0.3
0.0 0.0 0.0 0.0 0.5 0.5
0.0 0.0 0.0 0.0 0.0 0.0
<EndHMM>
EOF
done

cat > trainlist_dilili.txt <<EOF
mfcc/000_0.mfcc
mfcc/000_0.mfcc
mfcc/000_0.mfcc
EOF
cat > trainlist_sil.txt <<EOF
mfcc/000_1.mfcc
mfcc/000_1.mfcc
mfcc/000_1.mfcc
EOF

for i in dilili sil
do
HInit -S trainlist_${i}.txt -M train/init -H model/proto/hmm_${i}.hmm $i 
HRest -H train/init/hmm_${i}.hmm -S trainlist_${i}.txt -M train $i
done

cat > gram.txt<<EOF
\$WORD = DILILI;
( { SIL } [ \$WORD ] { SIL } )
EOF
cat > dict.txt <<EOF
DILILI  [dilili]  dilili
SIL      [sil]    sil
EOF

HParse gram.txt net.slf
HSGen -s net.slf dict.txt >/dev/null

# last column must add space
cat > hmmlist.txt <<EOF
dilili 
sil 
EOF

cat > testfile.txt <<EOF
test_0.sig   mfcc/test_0.mfcc
EOF
HCopy -C extract.conf  -S testfile.txt

HVite -H train/hmm_dilili.hmm  -H train/hmm_sil.hmm -i reco.mlf -w net.slf dict.txt hmmlist.txt mfcc/test_0.mfcc

if [ -e $TEMP_FILE ];then
	rm $TEMP_FILE
fi
