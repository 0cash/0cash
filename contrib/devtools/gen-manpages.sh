#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

0CASHD=${0CASHD:-$SRCDIR/0cashd}
0CASHCLI=${0CASHCLI:-$SRCDIR/0cash-cli}
0CASHTX=${0CASHTX:-$SRCDIR/0cash-tx}

[ ! -x $0CASHD ] && echo "$0CASHD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
ZECVERSTR=$($0CASHCLI --version | head -n1 | awk '{ print $NF }')
ZECVER=$(echo $ZECVERSTR | awk -F- '{ OFS="-"; NF--; print $0; }')
ZECCOMMIT=$(echo $ZECVERSTR | awk -F- '{ print $NF }')

# Create a footer file with copyright content.
# This gets autodetected fine for 0cashd if --version-string is not set,
# but has different outcomes for 0cash-cli.
echo "[COPYRIGHT]" > footer.h2m
$0CASHD --version | sed -n '1!p' >> footer.h2m

for cmd in $0CASHD $0CASHCLI $0CASHTX; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=$ZECVER --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-$ZECCOMMIT//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
