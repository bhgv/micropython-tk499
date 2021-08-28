#!/bin/bash

#echo "lpwd Script to upload examples using mpfshell"
#echo

ROOT=$PWD

upf_i=0



add_files () {
  local pp=$1
  local p="$1/"
  local is_ex="$2"
  local l=$(ls)
  local it

  local xx=30
  local yy=30

  local up_fn="${ROOT}/up.${upf_i}.mpf"

  echo > $up_fn

  upf_i=$((upf_i+1))

  if [ "$is_ex" == "" -a "$pp" == "exams" ];
  then
    is_ex=$pp
  fi

  if [ "$p" == "/" ];
  then
    p="";
  else
    if [ "$is_ex" != "" ];
    then
      echo "import lv_utils" > __init__.py
      echo "import lvgl as lv" >> __init__.py
      #echo "scr = lv.obj()" >> __init__.py
      echo >> __init__.py
    fi
  fi

  for it in $l
  do
    local x=$(echo $it | grep ".py")
    if [ "$x" != "" ];
    then
      echo "put ${p}${it} ${p}$it" >> $up_fn
      if [ "$p" != "" -a "$is_ex" != "" ];
      then
        if [ "$it" != "__init__.py" ];
        then
          local ito=$it
          it=$(echo $it | sed -r "s/^([01-9]+\.py)/p\1/")
          if [ "$it" != "$ito" ]
          then
            mv $ito $it
          fi
          echo "btn = lv.btn(lv.scr_act())" >> __init__.py
          echo "btn.set_height(lv.SIZE.CONTENT)" >> __init__.py
          echo "btn.align(lv.ALIGN.TOP_LEFT, ${xx}, ${yy})" >> __init__.py
          yy=$((yy+60))
          if [ $yy -gt 420 ]
          then
            yy=30
            xx=$((xx+150))
          fi
          local inm=$(echo $it | sed s/\.py$//)
          if [ "$pp" != "" ]
          then
            inm="$(echo $pp | sed "s/\//./g").${inm}"
          fi
          echo "def foo(e):"                               >> __init__.py
          echo "    if e.get_code() ==  lv.EVENT.CLICKED:" >> __init__.py
          echo "        lv_utils.ex_new_page(\"${p}${it}\")"  >> __init__.py
#          echo "        lv_utils.ex_new_page(\"${inm}\")"  >> __init__.py
#          echo "        import ${inm}"                     >> __init__.py
          echo "btn.add_event_cb(foo, lv.EVENT.ALL, None)" >> __init__.py
          echo "label = lv.label(btn)"                     >> __init__.py
          echo "label.set_text(\"${it}\")"                 >> __init__.py
          echo >> __init__.py
        fi
      fi
    fi
    local x=$(echo $it | grep ".png")
    if [ "$x" != "" ];
    then
      echo "put ${p}${it} ${p}$it" >> $up_fn
    fi
    local x=$(echo $it | grep ".fnt")
    if [ "$x" != "" ];
    then
      echo "put ${p}${it} ${p}$it" >> $up_fn
    fi
  done

  echo "cd /flash/$pp" >> $up_fn
  echo "ls" >> $up_fn
  echo "cd /flash" >> $up_fn

  for it in $l
  do
    if [ -d "$it" -a "$it" != "." -a "$it" != ".." -a "$it" != "-" ];
    then
      if [ "$p" != "" -a "$is_ex" != "" ];
      then
        echo "btn = lv.btn(lv.scr_act())" >> __init__.py
        echo "btn.set_height(lv.SIZE.CONTENT)" >> __init__.py
        echo "btn.align(lv.ALIGN.TOP_LEFT, ${xx}, ${yy})" >> __init__.py
        yy=$((yy+60))
        if [ $yy -gt 420 ]
        then
          yy=30
          xx=$((xx+150))
        fi
        local inm=$it
        if [ "$pp" != "" ]
        then
          inm="$(echo $pp | sed "s/\//./g").${inm}"
        fi
        echo "def foo(e):"                               >> __init__.py
        echo "    if e.get_code() ==  lv.EVENT.CLICKED:" >> __init__.py
        echo "        lv_utils.ex_new_page(\"${p}${it}/__init__.py\")"  >> __init__.py
#        echo "        lv_utils.ex_new_page(\"${inm}\")"  >> __init__.py
#        echo "        import $inm"                       >> __init__.py
        echo "btn.add_event_cb(foo, lv.EVENT.ALL, None)" >> __init__.py
        echo "label = lv.label(btn)"                     >> __init__.py
        echo "label.set_text(\"[${it}]\")"               >> __init__.py
        echo >> __init__.py
      fi

      echo "md ${p}${it}" >> $up_fn
      cd $it
      add_files "${p}${it}" "$is_ex"
      cd ..
    fi
  done

#  if [ "$p" != "" -a "$is_ex" != "" ];
#  then
#    echo "lv.scr_load(scr)" >> __init__.py
#  fi

}

add_files "" ""

echo "ls" >> ${ROOT}/up.0.mpf
