#must prepare file "configure.in", "Makefile.am" on current direction.
if [ -f "configure.in" ]; then
 echo "check file configure.in"
else
echo "configure.in not found"
exit 0
fi

if [ -f "Makefile.am" ]; then
 echo "ehck file Makefile.am"
else
echo "Makefile.am not found"
exit 0
fi

touch NEWS README AUTHORS ChangeLog

aclocal
autoconf
automake --add-missing
./configure
