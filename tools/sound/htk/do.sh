# from http://www.voxforge.org/home/dev/acousticmodels/windows/create/htkjulius/tutorial/data-prep/step-4
# from http://blog.csdn.net/jojozhangju/article/details/18821877

HTKTutorial=/home/abc/Public/tools/sound/htk/samples/HTKTutorial
#*/0001 DILILI
cat > prompts <<EOF
*/0001 DILILI
EOF

$HTKTutorial/prompts2wlist prompts wlist
cat >> wlist <<EOF
SENT-END
SENT-START
EOF

cat > global.ded <<EOF 
AS sp
RS cmu
MP sil sil sp
EOF

cat >> musics<<EOF
DILILI      [DILILI]       dilili
EOF

HDMan -A -D -T 1 -m -w wlist -n monophones0 -i -l dlog dict VoxForgeDict musics

$HTKTutorial/prompts2mlf words.mlf prompts

cat > mkphones0.led <<EOF
EX
IS sil sil
DE sp
EOF
HLEd -A -D -T 1 -l '*' -d dict -i phones0.mlf mkphones0.led words.mlf 

cat > wav_config <<EOF
SOURCEFORMAT = WAV
TARGETKIND = MFCC_0_D
TARGETRATE = 100000.0
SAVECOMPRESSED = T
SAVEWITHCRC = T
WINDOWSIZE = 250000.0
USEHAMMING = T
PREEMCOEF = 0.97
NUMCHANS = 26
CEPLIFTER = 22
NUMCEPS = 12
EOF
cat >codetrain.scp<<EOF
0001.wav    0001.mfc
EOF
HCopy -A -D -T 1 -C wav_config -S codetrain.scp 

cat > proto <<EOF
~o <VecSize> 25 <MFCC_0_D_N_Z>
~h "proto"
<BeginHMM>
  <NumStates> 5
  <State> 2
    <Mean> 25
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 
    <Variance> 25
      1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
 <State> 3
    <Mean> 25
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 
    <Variance> 25
      1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
 <State> 4
    <Mean> 25
      0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
    <Variance> 25
      1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0
 <TransP> 5
  0.0 1.0 0.0 0.0 0.0
  0.0 0.6 0.4 0.0 0.0
  0.0 0.0 0.6 0.4 0.0
  0.0 0.0 0.0 0.7 0.3
  0.0 0.0 0.0 0.0 0.0
<EndHMM>
EOF
cat > config <<EOF
TARGETKIND = MFCC_0_D_N_Z
TARGETRATE = 100000.0
SAVECOMPRESSED = T
SAVEWITHCRC = T
WINDOWSIZE = 250000.0
USEHAMMING = T
PREEMCOEF = 0.97
NUMCHANS = 26
CEPLIFTER = 22
NUMCEPS = 12 
EOF
awk '{print $2;}' codetrain.scp > train.scp
if [ ! -e hmm0 ];then
	mkdir hmm0
fi
HCompV -A -D -T 1 -C config -f 0.01 -m -S train.scp -M hmm0 proto

awk 'BEGIN{t=0;}{t++;if (t>4) print ;};END{}' hmm0/proto > temp_save.txt

a=hmm0/hmmdefs
if [ -e $a ];then
	rm $a
fi
while read line
do 
	echo "~h"  "\""$line"\"" >> $a
	cat  temp_save.txt >> $a
done < monophones0

a=hmm0/macros
awk 'BEGIN{t=0;};{t++;if (t < 4) print } END{}' hmm0/proto > $a
cat hmm0/vFloors >> $a

for i in 0 1 2 3
do
	if [ ! -e hmm$i ];then
		mkdir hmm$i
	fi
	HERest -A -D -T 1 -C config -I phones0.mlf -t 250.0 150.0 1000.0 -S train.scp -H hmm$i/macros -H hmm$i/hmmdefs -M hmm$((i+1)) monophones0

done

HVite -A -D -T 1 -l '*' -o SWT -b SENT-END -C config -H hmm4/macros -H hmm4/hmmdefs -i test1.mlf -m -t 250.0 150.0 1000.0 -y lab -a -I words.mlf -S train.scp dict monophones0

#HVite -A -D -T 1 -H hmm4/macros -H hmm4/hmmdefs -i reco.mlf -w wdnet  dict wlist 0002.mlf
HResults -I words.mlf  monophones0 test1.mlf
#end not need sp at all aaaaaaa
if [ 3 -eq 4 ]; then

if [ ! -e hmm4 ];then
	mkdir hmm4
fi
cp hmm3/macros hmm4/

awk 'BEGIN{f=0;tran=0;};{
if ($2 == "\"sp\"") f=1;
ignore=0;
if (f==1) if ($1 == "<NUMSTATES>"){ ignore=1;print $1"  "3}
else if ($1 == "<STATE>"){ print $1"  " 2;ignore=1}
else if ($1 == "<TRANSP>") {tran=1;} 
else if ($1 == "<ENDHMM>") {f=0;tran=0;} 

if (tran>0){ignore=1;if (tran==1){print $1"  "3} 
else if (tran==2){print "0.0 1.0 0.0";} 
else if (tran==3){print "0.0 0.9 0.1";} 
else if (tran==4){print "0.0 0.0 0.0";};tran++; } 

if (!ignore) print; 
};END{};' hmm3/hmmdefs >hmm4/hmmdefs

cat > sil.hed<<EOF
AT 2 4 0.2 {sil.transP}
AT 4 2 0.2 {sil.transP}
AT 1 3 0.3 {sp.transP}
TI silst {sil.state[3],sp.state[2]}
EOF

if [ ! -e hmm5 ]; then
	mkdir hmm5
fi
HHEd -A -D -T 1 -H hmm4/macros -H hmm4/hmmdefs -M hmm5 sil.hed monophones1

fi # e - eq 4
