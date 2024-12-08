INCLUDE1="./include"
INCLUDE2="./include/tensor"
INCLUDE3="./include/sformat"
INCLUDE4="./include/ann"
INCLUDE5="./demo"

MAIN="./src/program.cpp"
XTENSOR_LIB="./src/tensor/xtensor_lib.cpp"  

echo "################################################"
echo "# Compilation of the assignment: STARTED #######"
echo "################################################"


g++ -std=c++17 -I "$INCLUDE1" -I "$INCLUDE2" -I "$INCLUDE3" -I "$INCLUDE4" -I "$INCLUDE5" "$MAIN" "$XTENSOR_LIB" -o program

echo "################################################"
echo "# Compilation of the assignment: END     #######"
echo "# Binary file output: ./program ################"
echo "################################################"