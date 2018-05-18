#!/bin/bash

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

BERYCOIND=${BERYCOIND:-$SRCDIR/berycoind}
BERYCOINCLI=${BERYCOINCLI:-$SRCDIR/berycoin-cli}
BERYCOINTX=${BERYCOINTX:-$SRCDIR/berycoin-tx}
BERYCOINQT=${BERYCOINQT:-$SRCDIR/qt/berycoin-qt}

[ ! -x $BERYCOIND ] && echo "$BERYCOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
BERYVER=($($BERYCOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for bitcoind if --version-string is not set,
# but has different outcomes for bitcoin-qt and bitcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$BERYCOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $BERYCOIND $BERYCOINCLI $BERYCOINTX $BERYCOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BERYVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BERYVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m