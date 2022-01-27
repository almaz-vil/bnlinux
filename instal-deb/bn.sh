#!/bin/bash

text=$(echo "root" | sudo -S cat /tmp/bnlinux_sv)

echo "<txt> <span foreground='blue' weight='bold' size='large'>"$text"</span></txt>" 