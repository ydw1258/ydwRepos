using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Citizen : People
{
    public enum CITIZENSTATE
    {
        IDLE,
        WALK,
        RUN,
        DIE
    }
    // Start is called before the first frame update
    public CITIZENSTATE citizenState;
    public Animator anim;

    private float patternChangeTimer;
    private float patternChangeInterval = 3.0f;
    private float timer = 0;
    private int findRange = 10;
    
    //public
    public GameObject player;

    void Start()
    {
        player = GameObject.FindWithTag("Player");
        patternChangeTimer = patternChangeInterval;
    }
    // Start is called before the first frame update
    private void Update()
    {
        anim.SetInteger("CitizenState", (int)citizenState);
        TimerCheck();
        PatternChange(patternChangeInterval);
        if (citizenState == CITIZENSTATE.WALK)
            Move();
        else if(citizenState == CITIZENSTATE.RUN)
            RunAway();
    }
 
    void PatternChange(float patternChangeInterval)
    {
        patternChangeTimer += Time.deltaTime;

        if (patternChangeTimer > patternChangeInterval)
        {
            patternChangeTimer = 0.0f;
            if (citizenState == CITIZENSTATE.IDLE)
            {
                UpdatePeopleTargetDirection();
                citizenState = CITIZENSTATE.WALK;
            }
            else if (citizenState == CITIZENSTATE.WALK)
            {
                citizenState = CITIZENSTATE.IDLE;
            }
            else if (citizenState == CITIZENSTATE.RUN)
            {
                citizenState = CITIZENSTATE.IDLE;
            }
        }
    }
    void TimerCheck()
    {
        timer += Time.deltaTime;
        
        if (DectectedPlayerAttack())
        {
            SetRunAway();
        }
    }

    #region lowlevelCode
    bool DectectedPlayerAttack()
    {
        if (player.GetComponent<Player>().isAttack &&
            findRange > Vector3.Distance(transform.position, player.transform.position))
            return true;
        else
            return false;
    }
    void SetRunAway()
    {
        targetVector = player.transform.position;
        UpdatePeopleTargetDirection();
        citizenState = CITIZENSTATE.RUN;
        patternChangeTimer = 0.0f;
    }
    void UpdatePeopleTargetDirection()
    {
        transform.Rotate(0,Random.Range(0,360),0);
    }
    
    #endregion
}