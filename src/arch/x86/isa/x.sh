#!/bin/sh

for f in `grep -r 'UD2()' . | grep '.isa:' | grep -v 'x.sh' | cut -d: -f1 | uniq`; do

	echo "==> ${f}"

git checkout ${f}

	#sed -i .bak -e 's/Inst::UD2();/M5InternalError::error();/g' -e 's/ UD2();/ M5InternalError::error();/g' ${f}
	awk -v fn="${f}" '
		{
			s = sprintf("M5InternalError::error({{\"%s %d\"}})", fn, FNR);
			gsub(/Inst::UD2\(\)/, s);
			gsub(/ UD2\(\)/, sprintf(" %s", s));
			print $0;
		}
' ${f} > ${f}.x

mv ${f}.x ${f}

done

# end
