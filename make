rm -f argon2d/*.o
rm -f blake2/*.o
rm -f testhash
g++ -O3 -o testhash testhash.c argon2d/*.c blake2/*.c -lpthread
./testhash
