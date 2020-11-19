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
        teams[i].points=0; // initialising fields points, goalFor and goalAgainst to 0
        teams[i].goalFor=0;
        teams[i].goalAgainst=0;
    }
    fclose(file);
    return i; //number of lines = number of teams
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
    int i=0, match_date=0; //i is a counter
    char host_team[20], host_city[20], guest_team[20], guest_city[20];
    int goals_host=0, goals_guest=0;
    char c; 
    int lines=0;
    while((c = fgetc(file)) != EOF){
        if (c == '\n') {
        lines++; //define number if lines in file "match-result.txt"
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
                (compare_char_arrays(host_city, teams[count].city) == 1)){//comparing team's name from "match-results.txt" file with team's name from "teams.txt"
                //we are comparing the values to find where the host team is located in teams array --> to find its unique counter
                    find_num_of_array_host=1;
                    break;
            }
        count++;
        }
        int count2=0; //counter for guest team 
        while(count2<n){
            if ((compare_char_arrays(guest_team, teams[count2].name) == 1) && 
                (compare_char_arrays(guest_city, teams[count2].city) == 1)){ //comparing team's name from "match-results.txt" file with team's name from "teams.txt"
                    find_num_of_array_guest=1; //defining the counter of Guest array, where there is
                    break; 
            }
        count2++;
        }
        
        if(find_num_of_array_host==1 && find_num_of_array_guest==1){
            if(goals_host > goals_guest){ //adding points to teams
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
    return i; //number of teams processed
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
    int num_teams_max_points=0;//number of teams with similar maximum number of points
    int num_teams_max_goal_diff=0; //number of teams with similar maximum goal difference 
    for(i=0; i<n; i++){ 
        //this loop is mainly for printing the table 
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

    for(int k=0; k<n; k++){ //this loop is for defining number of teams with simialar maximum results
      if(teams[k].points==max_points){
        num_teams_max_points++;
      }
      if((teams[k].goalFor-teams[k].goalAgainst)==max_goal_diff){
        num_teams_max_goal_diff++;
      }
    }

    int j=0;
    while(j<n) { //this loop is for defining the chamption team 
      if(num_teams_max_points>1 && teams[j].points==max_points){
        if((num_teams_max_goal_diff>1 && (teams[j].goalFor-teams[j].goalAgainst)==max_goal_diff && teams[j].goalFor==max_goals) ||(num_teams_max_goal_diff==1 && (teams[j].goalFor-teams[j].goalAgainst)==max_goal_diff)){
            //return a pointer to the single champion team that has maximum goal difference OR  that has max goal scored
            return &teams[j];
            break;
        }
        else { 
            return  NULL; //it does not suit the criterias above ---> it means there are more than 1 champion team
            
        }
      }
      if (num_teams_max_points==1 && teams[j].points==max_points) {
        return &teams[j]; //return a pointer to the single champion team that has maximum number of points
       
          break;
      }

     j++;
    }


fclose(file); 
return &teams[j]; //– a pointer to the champion team
}

Match* storeResult(Team *host, Team *guest){
    char host_team[20], host_city[20], guest_team[20], guest_city[20];
    int goals_host=0, goals_guest=0, matchdate=0;

    FILE* file = fopen("match-results.txt", "r");
    char c;
    int j=0;

    do{
      
        fscanf(file, "%d %s %s %d %d %s %s", &matchdate, host_team, host_city, &goals_host, &goals_guest, guest_team, guest_city);
        
            if ((compare_char_arrays(guest_team, guest->name) == 1) && 
                (compare_char_arrays(host_team, host->name) == 1)){
                   //comparing every team's name from "match-results" with name from the struct array 
                    Match *matches = (Match*)malloc(sizeof(Match));  
                    if(matches==NULL){
                      return NULL;
                    }
                    else{
                        //initializing all the elements of matches to the values of file lines
                      matches->matchDay=matchdate;
                      matches->guest=guest;
                      matches->host=host;
                      matches->guestScore=goals_guest;
                      matches->hostScore=goals_host;
                      return matches;
                    }
            }
            
     
        } while(!feof(file));
        return NULL;
        fclose(file);
}

int comparePoints (const void * a, const void * b) //this function is used by qsort to compare points of teams
{
    Team *tms_pt1=(Team*)a;
    Team *tms_pt2=(Team*)b;
    return -(int)(tms_pt1->points-tms_pt2->points);
}

int compareScores(const void * a, const void * b){ //this function is used by qsort to compare goals scored of teams
    Team *tms_pt1=(Team*)a;
    Team *tms_pt2=(Team*)b;
    return -(int)(tms_pt1->goalFor-tms_pt2->goalFor);
}

int compareGoalDif(const void * a, const void * b){ //this function is used by qsort to compare goal differences of teams
    Team *tms_pt1=(Team*)a;
    Team *tms_pt2=(Team*)b;
    return -(int)((tms_pt1->goalFor-tms_pt1->goalAgainst)-(tms_pt2->goalFor-tms_pt2->goalAgainst));
}

int compareStrings(const void * a, const void * b){ //this function is used by qsort to compare strings --> to make them in alphabetic order
    return -strcmp(a, b);
}

void printOrderedStandings(int n, Team teams[32], char fileName[20]){
    FILE *file;
    file=fopen(fileName,"w");
    int i=0, j=0;
    _Bool dup_points=0; //initially there are no duplicates in number of points between teams
    _Bool dup_goalDiff=0; //no duplicates in goal difference between teams
    _Bool dup_score=0; //no duplicates in goals scored between teams
    
    Team sorted[32]; //create new Team struct array 
    for (int loop=0; loop<n; loop++) {
        sorted[loop] = teams[loop]; //copy data of teams to
    }
     //duplicates of points
    for(i=0; i<n; i++)
    {
        for(j=i+1;j<n;j++)
        {
            if(teams[i].points==teams[j].points)
             {
                dup_points=1;
                break;
            }
        }
   }

   for(i=0; i<n; i++)
    {
        for(j=i+1;j<n;j++)
        {
            if((teams[i].goalFor-teams[i].goalAgainst)==(teams[j].goalFor-teams[j].goalAgainst))
             {
                dup_goalDiff=1;
                break;
            }
        }
   }

   for(i=0; i<n; i++)
    {
        for(j=i+1;j<n;j++)
        {
            if(teams[i].goalFor==teams[j].goalFor)
             {
                dup_score=1;
                break;
            }
        }
   }

    if(dup_points==1){
        if(dup_goalDiff==1){
            if(dup_score==1){
                //sorting by alphabetic order as it is the last check
                qsort(sorted, n, sizeof(Team), compareStrings);
                printf("\nSorted by alphabetic order\n");
                
                
            }
            else if(dup_score==0){
                //sorting by goal scores as there are not duplications in scores
                qsort(sorted, n, sizeof(Team), compareScores);
                printf("\nSorted by goal scores\n");
            }
        }
        else if(dup_goalDiff==0){
            //sorting by goal difference as there are not duplicates in goal differences
            qsort(sorted, n, sizeof(Team), compareGoalDif);
            printf("\nSorted by goal difference\n");
        }
    }
    else if(dup_points==0){
        //sorting by points as there are not duplicates in points
        qsort(sorted, n, sizeof(Team), comparePoints);
        printf("\nSorted by points\n");
    }
    int goal_diff=0;
    printf("\nTEAM\t\t     P\t\t GF\t\t GA\t\t GD\n");
    for(i=0; i<n; i++){
        goal_diff=sorted[i].goalFor-sorted[i].goalAgainst;
        printf("%-10s\t\t %d\t\t %d\t\t %d\t\t %+d\n" , sorted[i].name, sorted[i].points, sorted[i].goalFor, sorted[i].goalAgainst, goal_diff);
    }
fclose(file);
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
    printf("\n------Task 4------\n");
    Match *match;
    //testing the function storeResult()
    match=storeResult(&teams[3], &teams[4]);
    int j=0;
    printf("%d %s %s %d %d\n", (*(match+j)).matchDay, (*(match+j)).host->name, (*(match+j)).guest->name, (*(match+j)).hostScore, (*(match+j)).guestScore);
    free(match);
    //
    //
    //TASK 5
    printf("\n------Task 5------\n");
    printOrderedStandings(i, teams, fileName);
    
    Team * champ;
    champ=printStandings(i, teams, fileName);
    return 0;
}
