export SYSTEMC_DISABLE_COPYRIGHT_MESSAGE=1

TEST_BIN=$1
TEST_CASE_COUNT=$($TEST_BIN --count)

PASS=0
FAIL=0

for i in $(seq 0 $(($TEST_CASE_COUNT - 1))); do
	$TEST_BIN test_case=$i;
	if [ $? -eq 0 ]; then
	  PASS=$((PASS + 1))
	else
	  FAIL=$((FAIL + 1))
	fi
done

echo ""
echo "Test Summary:"
echo "Failed: $FAIL / $TEST_CASE_COUNT"
echo "Passed: $PASS / $TEST_CASE_COUNT"

if  [ ! $FAIL -eq 0 ]; then
	exit 1
fi
