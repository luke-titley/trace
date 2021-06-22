# ------------------------------------------------------------------------------
# Copywrite Luke Titley 2015
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
#  Debug or Release ?
# ------------------------------------------------------------------------------

SSE=-mfpmath=sse -mmmx -msse -msse2 -msse3 -msse4 # -ftree-vectorize
DEBUG=-g -D _DEBUG -Werror -Wall $(SSE)
PROFILE=-g -O3 -Werror -Wall $(SSE)
RELEASE=-O3 -O3 -Werror -Wall $(SSE)
#CONFIGURATION=$(DEBUG)
#CONFIGURATION=$(PROFILE)
CONFIGURATION=$(RELEASE)
