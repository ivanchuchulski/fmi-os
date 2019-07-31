#!/bin/bash

name=$1

touch "${name}".sh
chmod u+x "${name}".sh

echo '#!/bin/bash' >> "${name}".sh