#!/bin/bash
# Purpose: Demonstrate usage of select and case with toggleable flags to indicate choices
# 2013-05-10 - Dennis Williamson

choice () {
    local choice=$1
    if [[ ${opts[choice]} ]] # toggle
    then
        opts[choice]=
    else
        opts[choice]=+
    fi
}

PS3='Please enter your choice: '
while :
do
    clear
   echo "Elija una opcion:"
    options=("Descargar todo del repositorio ${opts[1]}" "Clonar a otras maquinas ${opts[2]}" "Instalar CATEDRA ${opts[3]}"  "Instalar  TEAM ROCKET ${opts[4]}" "crearDiscoOsada  ${opts[5]}" "Abrir Pokedex Servidor  ${opts[6]}" "Abrir Pokedex Cliente  ${opts[7]}" "Finalizar Pokedex Cliente  ${opts[8]}"  "VAR_ENTORNO ${opts[9]}" "IP_PUERTO POKEDEX ${opts[10]}" "Salir")
    select opt in "${options[@]}"
    do
        case $opt in
            "Descargar todo del repositorio ${opts[1]}")
		choice 1
		echo "opcion1"
                sh ./1_download_SV.sh
		echo "Presione una tecla para continuar..."
		read -r A
                break
                ;;
            "Clonar a otras maquinas ${opts[2]}")
		choice 2
		echo "opcion2"
                sh ./2_clonar_offline.sh
		echo "Presione una tecla para continuar..."
		read -r A
                break
                ;;
            "Instalar CATEDRA ${opts[3]}")
		choice 3
		echo "opcion3"
                sh ./3_instalarCatedra.sh
		echo "Presione una tecla para continuar..."
		read -r A
                break
                ;;
            "Instalar  TEAM ROCKET ${opts[4]}")
		choice 4
		echo "opcion4"
                sh ./4_instalacionTeamRocket.sh
		echo "Presione una tecla para continuar..."
		read -r A
                break
                ;;
            "crearDiscoOsada  ${opts[5]}")
		choice 5
		echo "opcion5"
                sh ./5_crearDiscoOsada.sh
		echo "Presione una tecla para continuar..."
		read -r A
                break
                ;;
            "Abrir Pokedex Servidor  ${opts[6]}")
		choice 6
		echo "opcion6"
                sh ./6_abrirPokeServer.sh
		echo "Presione una tecla para continuar..."
		read -r A
                break
                ;;
            "Abrir Pokedex Cliente  ${opts[7]}")
		choice 7
		echo "opcion7"
                sh ./7_pokeCliente.sh
		echo "Presione una tecla para continuar..."
		read -r A
                break
                ;;
            "Finalizar Pokedex Cliente  ${opts[8]}")
		choice 8
		echo "opcion8"
                sh ./8_finalizarPokedexCliente.sh
		echo "Presione una tecla para continuar..."
		read -r A
                break
                ;;
            "VAR_ENTORNO ${opts[9]}")
		choice 9
		echo "opcion9"
                sh ./9_variableEntorno.sh
		echo "Presione una tecla para continuar..."
		read -r A
                break
                ;;
	"IP_PUERTO POKEDEX ${opts[10]}")
		choice 10
		echo "opcion10"
                sh ./10_IP_pokedex.sh
		echo "Presione una tecla para continuar..."
		read -r A
                break
                ;;
            "Salir")
                break 2
                ;;
            *) printf '%s\n' 'invalid option';;
        esac
    done
done

printf '%s\n' 'Opciones elegidas:'
for opt in "${!opts[@]}"
do
    if [[ ${opts[opt]} ]]
    then
        printf '%s\n' "Opciones $opt"
    fi
done