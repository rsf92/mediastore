#!/bin/bash

adduser mediastore --system --no-create-home

groupadd mms

usermod -aG mms mediastore

chown -R mediastore:mms /etc/mms
