LIBS=-lm

.PHONY: decode_pcm main clean pcm_to_wav

main:
	gcc dtw.c -o dtw $(LIBS)
	./dtw

DIR="/mnt/hgfs/Desktop/temp/autio_process/three"
clean:
	rm  $(DIR)/???.pcm

LIBS += -L/home/abc/Public/tools/libmad/_install/lib -lmad

decode_pcm:
	gcc decode_pcm.c -o decode_pcm ${LIBS}
	./decode_pcm

pcm_to_wav:
	gcc split_pcm_to_wav.c -o pcm_to_wav ${LIBS}
	./pcm_to_wav /mnt/hgfs/Desktop/temp/autio_process/japan_advance_level/Lesson1.mp3  \
	/mnt/hgfs/Desktop/temp/autio_process/three/Lesson1_1.wav \
	/mnt/hgfs/Desktop/temp/autio_process/three/Lesson1_2.wav \
	5198144
#	lame /mnt/hgfs/Desktop/temp/autio_process/three/Lesson1.wav /mnt/hgfs/Desktop/temp/autio_process/three/Lesson1.mp3
