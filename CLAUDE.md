# ft_malcolm - Contraintes du Sujet

## Contexte
**Projet**: ft_malcolm - Introduction to Man in the Middle attacks  
**Langage**: C uniquement  
**Correction**: Par humains (pas d'auto-correction)  
**Soumission**: Repository Git

---

## 🔧 Instructions de Workflow

- Quand l'utilisateur dit **"push"**, exécuter `git push` (commit au préalable si nécessaire, puis push vers le remote).
- Quand l'utilisateur dit **"push"** en fournissant le message de commit, commiter avec ce message et pusher **directement**, sans attendre de confirmation/clic supplémentaire.
- **Avant chaque push**, mettre à jour la roadmap dans `documentation.txt`.
- Code : utiliser des **tabulations** pour l'indentation, jamais d'espaces.
- Code : **jamais de `return` direct d'une expression** — toujours stocker le résultat dans une variable puis retourner la variable.
  - ❌ `return (c - '0');`
  - ✅ `value = c - '0'; return (value);`
- **Laisser l'utilisateur essayer d'abord pendant 15 minutes** : à la place, donner un **cours** (explication + petits exemples de code illustratifs) sur la fonction/notion demandée, sans écrire la fonction complète. Après ces 15 minutes, du code peut être donné, mais l'utilisateur doit d'abord bien comprendre l'explication avant d'accepter ce code.

## 🎓 Style des cours

- Quand l'utilisateur demande un cours/explication : **aller à l'essentiel**, pas de blabla.
- Donner un **exemple de code simple** quand c'est pertinent.
- **Donner le cours étape par étape**, chaque étape étant **courte**. Ne pas tout expliquer d'un coup : si l'utilisateur a besoin de plus de détails, c'est lui qui posera des questions.

---

## ⚠️ CONTRAINTES STRICTES

### Langage & Compilation
- **Langage**: C uniquement
- **Makefile**: OBLIGATOIRE avec règles standards (all, clean, fclean, re)
- **Executable**: Nommé `ft_malcolm`
- **Compilation**: Doit compiler sans erreurs

### Architecture Code
- **Variables globales**: 1 SEULE autorisée dans tout le projet
- **Virtual Machine**: OBLIGATOIRE (Linux kernel > 3.14, testé sur Debian 7.0)
- **Erreurs**: ZÉRO TOLÉRANCE - Pas de crash (Segmentation fault, bus error, double free)

### Fonctions AUTORISÉES (Mandatory)
```
sendto, recvfrom
socket, setsockopt
inet_pton, inet_ntop
if_nametoindex, sleep
getuid, close
sigaction, signal
inet_addr
gethostbyname
getaddrinfo, freeaddrinfo
getifaddrs, freeifaddrs
htons, ntohs
strerror / gai_strerror
printf et sa famille
libft functions
```

---

## 📋 REQUIREMENTS (Mandatory Part)

### Arguments & Entrée
- **4 arguments OBLIGATOIRES dans cet ordre**:
  1. `source_ip` - Adresse IP à usurper
  2. `source_mac` - Adresse MAC associée
  3. `target_ip` - Adresse IP de la cible
  4. `target_mac` - Adresse MAC de la cible

### Comportement
- ✅ Attendre une **ARP request** envoyée en broadcast par la cible (demandant source_ip)
- ✅ Dès qu'une requête est reçue, envoyer **UNE SEULE ARP reply** falsifiée
- ✅ **Quitter immédiatement après** l'envoi
- ✅ Pouvoir être quitté avec **Ctrl+C** pendant l'attente

### Résultat Attendu
Après exécution, la table ARP de la cible doit contenir:
```
source_ip → source_mac (falsifiée)
```

---

## 🛡️ GESTION D'ERREURS

### Validations OBLIGATOIRES
```
Adresse IP invalide → "ft_malcolm: unknown host or invalid IP address: (X)"
Adresse MAC invalide → "ft_malcolm: invalid mac address: (X)"
```

### Pas de Crash
- Segmentation fault ❌
- Bus error ❌
- Double free ❌
- Buffer overflow ❌
- Accès mémoire invalide ❌

---

## 📝 CHECKLIST AVANT SOUMISSION

- [ ] Executable nommé `ft_malcolm`
- [ ] Makefile avec règles standards
- [ ] 1 seule variable globale max
- [ ] Compilation sans erreurs
- [ ] Zéro crash (pas de SIGSEGV, etc)
- [ ] 4 arguments traités dans l'ordre
- [ ] ARP spoofing fonctionnel
- [ ] Quitter après ARP reply
- [ ] Gestion Ctrl+C
- [ ] Messages d'erreur exacts
- [ ] Validations IP et MAC
- [ ] Code dans repository Git
- [ ] VM Linux kernel > 3.14

---

## 🚀 Notes Importantes

1. **Prototype**: Lire RFC 826 et 7042 sur le protocole ARP
2. **Raw Sockets**: Créer des paquets ARP from scratch
3. **Network Byte Order**: Utiliser htons/ntohl pour conversions
4. **Interface réseau**: Détecter l'interface disponible
5. **Seulement IPv4** (Mandatory) - Pas IPv6
6. **Spoofing sécurisé**: Seules les IPs de ta VM, pas d'autres machines

---

**Version**: 3  
**Créé**: Pour ft_malcolm  
**Mise à jour**: Avant chaque session de dev
