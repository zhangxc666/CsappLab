s=$1
txt="./trace"$1".txt"
make
echo "---------------------------------"
echo "--------------tsh----------------"
perl ./sdriver.pl -t $txt -s ./tsh -a "-p"
echo "--------------tsh----------------"
echo "---------------------------------"
echo ""
echo ""
echo "---------------------------------"
echo "-------------tshref--------------"
perl ./sdriver.pl -t $txt -s ./tshref -a "-p"
echo "-------------tshref--------------"
echo "---------------------------------"
