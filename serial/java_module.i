#if defined(SWIGJAVA)

%typemap(in)     (char * BYTE, int LENGTH) {
/* Functions from jni.h */
$1 = (char *) JCALL2(GetByteArrayElements, jenv, $input, 0);
$2 = (int)    JCALL1(GetArrayLength,       jenv, $input);
}
%typemap(jni)    (char * BYTE, int LENGTH) "jbyteArray"
%typemap(jtype)  (char * BYTE, int LENGTH) "byte[]"
%typemap(jstype) (char * BYTE, int LENGTH) "byte[]"
%typemap(javain) (char * BYTE, int LENGTH) "$javainput"

/* Specify signature of method to handle */
%apply (char * BYTE, int LENGTH)   { (char * byteArray, long len) };

#else
%apply (char * STRING, int LENGTH) { (char *  byteArray, long len) };
#endif
