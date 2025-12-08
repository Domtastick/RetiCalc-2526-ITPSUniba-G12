## Informazioni sul sistema d'origine
![Screenshot di fastfetch](https://domfantastick.s-ul.eu/h75571Mg.png)
<details>
<summary><b>Informazioni generali e build instruction</b></summary>

### Commenti nel codice
Ho iniziato il progetto creando in primis la parte **TCP** ed in seguito la parte **UDP** applicando qualche modifica sul codice originale TCP.
Pertanto i commenti nei source file UDP fanno spesso riferimento a cose in quelli TCP, indi per cui è meglio ispezionare prima il codice TCP.

### Decisioni implementative
Essendo il goal finale quello di creare una calcolatrice trovavo la sintassi di **LETTERA**: **V1**,**V2** poco intuitiva.
Nella mia implementazione ho preferito usare una sintassi più intuitiva e simile ad una calcolatrice, ovvero **V1** **OPERATORE** **V2**.
Tutte le informazioni necessarie verranno anche printate dai client nello standard output.

### Struttura file
Per questioni di praticità sia implementativa che compilativa ho creato dei source file per **client** e **server** ed un source file condiviso **utility**
contenente funzioni generiche necessarie a entrambi. I source file per entrambi i casi sono presenti nelle loro rispettive cartelle denominate col nome del caso + id del gruppo.

Una volta compilato il progetto, verrà creata una parent folder **bin** contenente i binary di **client** e **server** in delle apposite child folder **TCP** e **UDP**.
Ignorando gli extra file che verranno creati da cmake per il processo di building, la struttura finale dei file del progetto sarà come nello screenshot:

![Screenshot della struttura file](https://domfantastick.s-ul.eu/iCqEl0zo.png)

<details>
<summary><b>Istruzioni di building</b></summary>

Come prima cosa è necessario che il sistema abbia l'utility <b>make</b> già installata. Se le xcode utility sono già installate non dovrebbero esserci problemi; in caso contrario si possono installare col seguente comando:

```sh
xcode-select --install
```

Una volta installate le utility xcode bisogna installare <b>Cmake</b>. Il metodo più veloce per farlo e che consiglio maggiormente è tramite l'ausilio di un package manager come <b>Homebrew</b>.
In caso non lo abbiate già installato, potete seguire <a href="https://brew.sh/">questa guida</a> sul sito ufficiale.

Si può poi procedere ad installare Cmake:

```sh
brew install cmake
```

Poi procedete con navigare nel root directory del progetto usando il vostro terminal emulator di scelta e runnate il seguente comando per generare il makefile dalla configurazione cmake:

```sh
cmake .
```

Creato il makefile basta eseguire il comando make ed avrete finito!

```sh
make 
```

Inoltre, <b>tutti</b> i file in più creati da cmake (incluso il makefile) possono essere eliminati se si desidera runnando il seguente comando:

```sh
rm -r *ake*
```

</details>
  
</details>
