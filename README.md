# ft_malcolm

Introduction aux attaques Man In The Middle via ARP spoofing.

Le programme usurpe une adresse IP (`source_ip`) en répondant à une requête ARP
la concernant avec une adresse MAC falsifiée (`source_mac`). Il envoie une
seule réponse ARP falsifiée à la cible (`target_ip` / `target_mac`), puis quitte.

## Compilation

```sh
make
```

Règles disponibles : `all`, `clean`, `fclean`, `re`.

## Utilisation

```sh
./ft_malcolm <source_ip> <source_mac> <target_ip> <target_mac>
```

- `source_ip` / `source_mac` : l'identité usurpée (l'IP, et le faux MAC qu'on
  lui associe)
- `target_ip` / `target_mac` : la victime, celle dont on va empoisonner la
  table ARP

Le programme doit être lancé en root, et attend qu'une requête ARP demandant
`source_ip` soit diffusée sur le réseau avant de répondre et de quitter.

## Gestion des erreurs

- Lancé sans les droits root : message d'erreur, le programme quitte.
- Lancé en root sans les 4 arguments : usage affiché.
- IP invalide : `ft_malcolm: unknown host or invalid IP address: (X).`
- MAC invalide : `ft_malcolm: invalid mac address: (X)`
- Ctrl+C pendant l'attente : quitte immédiatement.

## Tester le projet

### Pré-requis

Le sujet impose de tout faire dans une VM Linux (kernel > 3.14). Toutes les
commandes ci-dessous s'exécutent donc depuis cette VM.

### Pourquoi Docker plutôt que le réseau physique

Tester un spoofing ARP entre la VM et une vraie machine (host de l'école, par
exemple) ne fonctionne pas forcément : la plupart des réseaux d'établissement
ont des protections (Dynamic ARP Inspection, port security) qui bloquent ou
neutralisent les trames ARP falsifiées avant qu'elles n'atteignent la cible.

La solution adoptée ici : utiliser Docker, **à l'intérieur de la VM**, pour
créer un mini réseau isolé avec deux machines virtuelles légères (conteneurs)
qui ne dépendent pas du réseau de l'école.

### Mise en place

Deux conteneurs sont définis dans `docker-compose.yml`, sur un réseau bridge
isolé (`172.20.0.0/24`) :

- `attacker` (`172.20.0.10`) : contient le binaire `ft_malcolm` compilé
- `target` (`172.20.0.20`) : la victime, dont on va vérifier la table ARP

Des cibles dédiées sont ajoutées dans le `Makefile` :

```sh
make drun        # build (sans cache) + démarre les 2 conteneurs
make dattacker    # ouvre un shell dans le conteneur attacker
make dtarget      # ouvre un shell dans le conteneur target
make ddown        # arrête et supprime les conteneurs
```

### Déroulé d'un test complet

1. Démarrer l'environnement :

   ```sh
   make drun
   ```

2. Récupérer l'adresse MAC du conteneur `target` :

   ```sh
   make dtarget
   ip addr show eth0
   ```

3. Dans un autre terminal, lancer `ft_malcolm` depuis `attacker`, en
   usurpant une IP qui n'appartient à aucun conteneur (ex: `172.20.0.50`,
   pour éviter qu'une vraie réponse ne vienne concurrencer la fausse) :

   ```sh
   make dattacker
   ./ft_malcolm 172.20.0.50 aa:bb:cc:dd:ee:ff 172.20.0.20 <mac de target>
   ```

   Le programme attend une requête ARP demandant `172.20.0.50`.

4. Dans le terminal `target`, déclencher cette requête :

   ```sh
   ping -c1 172.20.0.50
   ```

   `ft_malcolm` doit afficher la requête reçue, envoyer la fausse reply, puis
   quitter.

5. Vérifier que `target` a bien été trompé :

   ```sh
   arp -a
   ```

   Le MAC affiché doit être `aa:bb:cc:dd:ee:ff` (le faux), pas un vrai MAC.

### Observer le trafic ARP (tcpdump)

Pour vérifier que la reply n'est envoyée qu'après une requête ARP sur le
réseau, on écoute le trafic depuis la VM (hors conteneurs), sur l'interface
bridge créée par Docker pour `malcolm-net` :

```sh
ip link show | grep br-
sudo tcpdump -vv -i br-xxxxxxxxxxxx arp
```

Refaire les étapes 3 et 4 ci-dessus : on doit voir d'abord la `Request`, puis
la `Reply` falsifiée juste après.

### Tester la gestion des erreurs

Depuis le conteneur `attacker` :

```sh
./ft_malcolm 999.999.999.999 aa:bb:cc:dd:ee:ff 172.20.0.20 <mac de target>
./ft_malcolm 172.20.0.50 zz:zz:zz:zz:zz:zz 172.20.0.20 <mac de target>
```

Les deux doivent afficher un message d'erreur exact et quitter sans crash.
