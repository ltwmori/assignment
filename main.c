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

typedef struct{
    int matchDay;
    Team *host;
    Team *guest;
    int hostScore;
    int guestScore;
}
Match;


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
    int num_teams_max_points=0;
    int num_teams_max_goal_diff=0; //number of teams with maximum number of points
    for(i=0; i<n; i++){
        goal_diff=teams[i].goalFor-teams[i].goalAgainst;

        fprintf(file,"%-10s\t\t %d\t\t %d\t\t %d\t\t %+d\n" , teams[i].name, teams[i].points, teams[i].goalFor, teams[i].goalAgainst, goal_diff);
        printf("%-10s\t\t %d\t\t %d\t\t %d\t\t %+d\n" , teams[i].name, teams[i].points, teams[i].goalFor, teams[i].goalAgainst, goal_diff);

        if(teams[i].points>max_points){ //max points
            max_points=teams[i].points;
        }

        if(goal_diff>max_goal_diff){ //max goal difference
            max_goal_diff=goal_diff;
        }

        if(teams[i].goalFor>max_goals){ //max scored goals
            max_goals=teams[i].goalFor;
        }
    }
printf("max points is %d\n", max_points);
printf("max goal diff is %d\n", max_goal_diff);
printf("max scored goals is %d\n", max_goals);
    for(int k=0; k<n; k++){
      if(teams[k].points==max_points){
        num_teams_max_points++;
      }
      if((teams[k].goalFor-teams[k].goalAgainst)==max_goal_diff){
        num_teams_max_goal_diff++;
      }
    }

    printf("number of teams with the same max points is %d", num_teams_max_points++);
    printf("number of teams with the same max goal diss is %d", num_teams_max_goal_diff);
    int j=0;
    while(j<n) {
      if(num_teams_max_points>1){
        if(((num_teams_max_goal_diff>1) && (teams[j].goalFor==max_goals)) ||(num_teams_max_goal_diff==1)){
              
            return &teams[j];
            break;
        }
        else {
            return 0;
            
        }
      }
      if (num_teams_max_points==1) {
        return &teams[j];
          break;
      }

     j++;
    }


fclose(file); 
return &teams[j];
}

Match* storeResult(Team *host, Team *guest){
    char host_team[20], host_city[20], guest_team[20], guest_city[20];
    int goals_host=0, goals_guest=0, matchdate=0;

    FILE* file = fopen("match-results.txt", "r");
    char c;
    int j=0;
Match *matches = (Match*)malloc(sizeof(matches[j])); 
    for(j=0; ((c = fgetc(file)) != EOF); j++){
      
        fscanf(file, "%d %s %s %d %d %s %s", &matchdate, host_team, host_city, &goals_host, &goals_guest, guest_team, guest_city);
        
            if ((compare_char_arrays(guest_team, guest->name) == 1) && 
                (compare_char_arrays(host_team, host->name) == 1)){
                   
                     
                    if(matches==NULL){
                      return NULL;
                    }
                    else{
                      (*(matches+j)).matchDay=matchdate;
                      strcpy(matches->guest->name, guest_team);
                      strcpy(matches->host->name, host_team);
                      (*(matches+j)).guestScore=goals_guest;
                      (*(matches+j)).hostScore=goals_host;
                      return &matches[j];
                    }
            }
            
     
        }
    return matches;
}



int main(void){
    
    char firstName[] = "Assel";
    char lastName[] = "Abzalova";
    char studentID[] = "201966956";
    printf("Student: %s %s\n\n", firstName, lastName);
    Team teams[32];
    //
    //
    //TASK 1
    printf("------Task 1------\n");
    int i = initTeams(teams);
    printf("The number of entries stored in array is %d\n", i);
    //Testing the initTeams function - outputs the list of teams
    for (int co=0; co<i; ++co){
      printf("%s %s\n", teams[co].name, teams[co].city);
    }
    //
    //TASK 2
    printf("\n------Task 2------\n");
    //Testing addResults function --> outputting the points of each team
    int matches = addResults(i, teams);
    printf("\nThe number of matches processed is %d\n", matches);
    //
    //
    //TASK 3
    printf("\n------Task 3------\n");
    char fileName[20];
    printf("\nInput file name\n");
    scanf("%s", fileName); //user writes the name of the file
    Team * tms;
    tms = printStandings(i, teams, fileName);
    printf("\nThe champion team is %s\n", tms->name);
    //
    //
    //TASK 4
    /*
    printf("\n------Task 4------\n");
    Match *matches_from_file;
    //testing the function storeResult()
    matches_from_file=storeResult(&teams[3], &teams[4]);
    printf("printing the values were sizeof\n");
    for(int j=0; j<(sizeof(matches_from_file)/sizeof(matches_from_file[0])); j++){
      printf("%d %s %s %d %d\n", (*(matches_from_file+j)).matchDay, (*(matches_from_file+j)).host->name, (*(matches_from_file+j)).guest->name, (*(matches_from_file+j)).hostScore, (*(matches_from_file+j)).guestScore);
    }
    
    
    free(matches_from_file);
*/

    return 0;
}
