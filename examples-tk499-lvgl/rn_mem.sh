i=1; e=18; e=$((e+1)); while [ $i -lt $e ]; do mv $(ls *_${i}.py) ${i}.py ; i=$((i + 1)); done; rm *.c *.h; touch __init__.py
