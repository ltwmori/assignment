/*

Assel Abzalova
ID: 201966956

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
   char name[20];
   char city[20];
   int points;
   int goalFor;
   int goalAgainst;  
}
Team;

int initTeams(Team teams[32]){
    FILE *file = fopen("teams.txt", "r");
    int i=0, lines=0;
    char c; 
    while ((c = fgetc(file)) != EOF) { //define overall number of lines in the file

        if (c == '\n') {
            lines++;
        }
        
    }
    fclose(file);

    file = fopen("teams.txt", "r");
    for(i=0; i<=lines; ++i){
        fscanf(file, "%s %s", teams[i].name, teams[i].city);
        teams[i].points=0;
        teams[i].goalFor=0;
        teams[i].goalAgainst=0;
    }
    fclose(file);
    return i;
}


int compare_char_arrays(char s1[], char s2[]) { //function to compare arrays
   int i = 0;
   int r = 0;

   while (s1[i] == s2[i] && r == 0) {
      if (s1[i] == '\0' || s2[i] == '\0') {
         r = 1;
      }
      i++;
   }
   return r;
}


int addResults(int n, Team teams[32]){
    FILE *file=fopen("match-results.txt", "r");
    int i=0, match_date=0;
    char host_team[20], host_city[20], guest_team[20], guest_city[20];
    int goals_host=0, goals_guest=0;
    char c;
    int lines=0;
    while((c = fgetc(file)) != EOF){
        if (c == '\n') {
        lines++;
      }
    }

    fclose(file);

    file=fopen("match-results.txt", "r");

    while(i<=lines){
        fscanf(file, "%d %s %s %d %d %s %s", &match_date, host_team, host_city, &goals_host, &goals_guest, guest_team, guest_city);
        int count=0; //counter for host team (array)
        _Bool find_num_of_array_host=0;
        _Bool find_num_of_array_guest=0;
        while (count<n){
            if ((compare_char_arrays(host_team, teams[count].name) == 1) && 
                (compare_char_arrays(host_city, teams[count].city) == 1)){
                    find_num_of_array_host=1;
                    break;
            }
        count++;
        }
        int count2=0; //counter for guest team 
        while(count2<n){
            if ((compare_char_arrays(guest_team, teams[count2].name) == 1) && 
                (compare_char_arrays(guest_city, teams[count2].city) == 1)){
                    find_num_of_array_guest=1;
                    break; 
            }
        count2++;
        }
        
        if(find_num_of_array_host==1 && find_num_of_array_guest==1){
            if(goals_host > goals_guest){
                teams[count].points+=3;
                teams[count].goalFor+=goals_host;
                teams[count].goalAgainst+=goals_guest;
                teams[count2].goalFor+=goals_guest;
                teams[count2].goalAgainst+=goals_host;
            } 

            else if(goals_host < goals_guest){
                teams[count2].points+=3;
                teams[count2].goalFor+=goals_guest;
                teams[count2].goalAgainst+=goals_host;
                teams[count].goalFor+=goals_host;
                teams[count].goalAgainst+=goals_guest;
            }

            else if(goals_host == goals_guest){
                teams[count].points+=1;
                teams[count2].points+=1;

            }
        }
        
        i++;      
        
        
    }
    fclose(file);
    return i;
}

Team* printStandings(int n, Team teams[32], char fileName[20]){
    FILE *file;
    file=fopen(fileName,"w");
    int max_points=0;
    fprintf(file, "\nTEAM\t\t     P\t\t GF\t\t GA\t\t GD\n");
    printf("\nTEAM\t\t     P\t\t GF\t\t GA\t\t GD\n");
    int i=0;
    int goal_diff=0, max_goal_diff=0;
    int max_goals=0;
    int num_teams_max_points=0; //number of teams with maximum number of points
    for(i=0; i<n; i++){
        goal_diff=teams[i].goalFor-teams[i].goalAgainst;
        fprintf(file,"%-10s\t\t %d\t\t %d\t\t %d\t\t %+d\n" , teams[i].name, teams[i].points, teams[i].goalFor, teams[i].goalAgainst, goal_diff);
        printf("%-10s\t\t %d\t\t %d\t\t %d\t\t %+d\n" , teams[i].name, teams[i].points, teams[i].goalFor, teams[i].goalAgainst, goal_diff);

        if(teams[i].points>max_points){
            max_points=teams[i].points;
        }

        if(goal_diff>max_goal_diff){
            max_goal_diff=goal_diff;
        }

        if(teams[i].goalFor>max_goals){
            max_goals=teams[i].goalFor;
        }
    }

    for(int i=0; i<n; i++){
        if(teams[i].points==max_points){
        num_teams_max_points++;
        }
        if(num_teams_max_points>1){
            if((teams[i].goalFor-teams[i].goalAgainst)==max_goal_diff){
                if(teams[i].goalFor==max_goals){
                    return &teams[i];
                    break;
                }
                else {
                    return NULL;
                    break;
                }
            }
            
        }
        else if (num_teams_max_points==1) {
            return &teams[i];
            break;
        }
    }


fclose(file);      
return &teams[i];
}

int main(void){
    
    char firstName[] = "Assel";
    char lastName[] = "Abzalova";
    char studentID[] = "201966956";
    printf("Student: %s %s\n\n", firstName, lastName);
    Team teams[32];
    //
    //TASK 1
    //
    int i = initTeams(teams);
    printf("The number of entries stored in array is %d\n", i);
    //Testing the initTeams function - outputs the list of teams
    for (int co=0; co<i; ++co){
      printf("%s %s\n", teams[co].name, teams[co].city);
    }
    //
    //TASK 2
    //Testing addResults function --> outputting the points of each team
    int matches = addResults(i, teams);
    printf("\nThe number of matches processed is %d\n", matches);
    //
    //
    //TASK 3
    char fileName[20];
    printf("\nInput file name\n");
    scanf("%s", fileName); //user writes the name of the file
    Team * tms = printStandings(i, teams, fileName);

    return 0;
}
