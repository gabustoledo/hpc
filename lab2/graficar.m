N =256;

f1 = fopen('salidas/aqui.raw','r');
I1 = fread(f1,N*N,'float32');
I1 = reshape(I1,N,N);
I1 = I1';
fclose('all');
imagesc(I1); axis('off');axis('square');