# Scénario de démonstration pour TechInterventionPlanner

Voici un scénario d'utilisation étape par étape pour démontrer les fonctionnalités principales et les design patterns implémentés dans l'application TechInterventionPlanner.

## Préparation de la démo

1. Compilez et lancez l'application
2. Assurez-vous d'avoir les utilisateurs test prêts (admin, manager, tech)

## Scénario de démonstration

### 1. Authentification et sécurité (démonstration du Proxy Pattern)

1. Démarrez l'application et observez l'écran d'accueil
```
=== INTERVENTION MANAGER ===
Type 'help' for a list of commands.
>
```

2. Essayez d'accéder aux interventions sans être connecté
```
> list
Please login first.
```

3. Connectez-vous en tant que technicien
```
> login tech tech123
Login successful. Welcome, tech (technician).
```

4. Essayez de créer une intervention (démonstration de la limitation des droits)
```
> create Maintenance "Office Building A" "2025-04-25 10:30" 120
Permission denied. Only admins and managers can create interventions.
```

5. Déconnectez-vous et connectez-vous en tant qu'administrateur
```
> logout
Logged out.
> login admin admin123
Login successful. Welcome, admin (admin).
```

### 2. Création et gestion des interventions (démonstration du Factory et Facade Pattern)

1. Créez une intervention de maintenance
```
> create Maintenance "Office Building A" "2025-04-25 10:30" 120
Intervention created with ID: 1
```

2. Créez une intervention d'urgence
```
> create Emergency "Data Center B" "2025-04-25 14:45" 60
Intervention created with ID: 2
```

3. Affichez la liste des interventions pour cette date
```
> list 2025-04-25
Interventions for 2025-04-25:
ID | Type        | Location        | Time   | Duration | Technician | Status
----+-------------+-----------------+--------+----------+------------+----------
1  | Maintenance | Office Building A | 10:30  | 120      |            | Scheduled
2  | Emergency   | Data Center B    | 14:45  | 60       |            | Scheduled
```

### 3. Ajout de techniciens et assignation (démonstration de la Facade)

1. Ajoutez quelques techniciens
```
> addtech TECH001 "John Smith" "Electrical" "john@example.com"
Technician added.
User account created for technician. Default password: TECH001123

> addtech TECH002 "Jane Doe" "Network" "jane@example.com"
Technician added.
User account created for technician. Default password: TECH002123
```

2. Listez les techniciens disponibles
```
> techs
Technicians:
ID       | Name           | Specialty      | Contact
---------+----------------+----------------+----------------
TECH001  | John Smith     | Electrical     | john@example.com
TECH002  | Jane Doe       | Network        | jane@example.com
```

3. Assignez les techniciens aux interventions
```
> assign 1 TECH001
Technician assigned.

> assign 2 TECH002
Technician assigned.
```

4. Vérifiez les interventions mises à jour
```
> list 2025-04-25
Interventions for 2025-04-25:
ID | Type        | Location        | Time   | Duration | Technician | Status
----+-------------+-----------------+--------+----------+------------+----------
1  | Maintenance | Office Building A | 10:30  | 120      | TECH001    | Scheduled
2  | Emergency   | Data Center B    | 14:45  | 60       | TECH002    | Scheduled
```

### 4. Modification du statut et observation (démonstration de l'Observer Pattern)

1. Modifiez le statut d'une intervention
```
> status 2 "In Progress" "Work started on network issues"
Status updated.
[NOTIFICATION] Intervention 2 status changed from Scheduled to In Progress
```

2. Examinez les détails de l'intervention
```
> view 2
Intervention #2
Type: Emergency
Location: Data Center B
Date: 2025-04-25 14:45
Duration: 60 minutes
Technician: TECH002
Status: In Progress
Comments: Work started on network issues
```

### 5. Consultation du calendrier (fonctionnalité additionnelle)

1. Affichez le calendrier mensuel
```
> calendar 4 2025
April 2025
Mo Tu We Th Fr Sa Su
    1  2  3  4  5  6
 7  8  9 10 11 12 13
14 15 16 17 18 19 20
21 22 23 24 25* 26 27
28 29 30
* = Day with interventions
```

### 6. Décorateurs (démonstration du Decorator Pattern)

Remarque : Cette partie est généralement implémentée dans le code, mais peut être démontrée via les sorties ou des commandes spéciales si elles ont été ajoutées à l'interface CLI.

```
> decorate 1 gps
GPS tracking added to intervention #1.

> decorate 1 attachments
Attachments capability added to intervention #1.

> addattachment 1 "wiring_diagram.pdf" "Building electrical layout"
Attachment added to intervention #1.

> view 1
Intervention #1
Type: Maintenance
Location: Office Building A
Date: 2025-04-25 10:30
Duration: 120 minutes
Technician: TECH001
Status: Scheduled
GPS Tracking: Enabled (Update frequency: 15 minutes)
Attachments: 1
Files:
- wiring_diagram.pdf (Building electrical layout) [Added: 04/22/2025 11:30:45]
```

### 7. Persistence des données (fonctionnalité de la Facade)

1. Sauvegardez les données
```
> savedata
Data saved successfully.
```

2. Déconnectez-vous et reconnectez-vous
```
> logout
Logged out.
> login admin admin123
Login successful. Welcome, admin (admin).
```

3. Chargez les données
```
> loaddata
Data loaded successfully.
```

4. Vérifiez que les interventions sont toujours là
```
> list 2025-04-25
Interventions for 2025-04-25:
ID | Type        | Location        | Time   | Duration | Technician | Status
----+-------------+-----------------+--------+----------+------------+----------
1  | Maintenance | Office Building A | 10:30  | 120      | TECH001    | Scheduled
2  | Emergency   | Data Center B    | 14:45  | 60       | TECH002    | In Progress
```

### 8. Clôture de la démo

```
> exit
Exiting application.
```

Ce scénario permet de démontrer toutes les fonctionnalités principales de l'application et met en évidence l'utilisation de chaque design pattern implémenté :

- **Factory Method** : Création de différents types d'interventions
- **Decorator** : Ajout dynamique de fonctionnalités GPS et pièces jointes
- **Facade** : Interface unifiée pour toutes les opérations complexes
- **Observer** : Notifications lors des changements d'état
- **Proxy** : Contrôle d'accès et gestion des permissions

Pour une démo réussie, vous pouvez préparer ce script à l'avance et vous assurer que toutes les commandes fonctionnent comme prévu avant la présentation.