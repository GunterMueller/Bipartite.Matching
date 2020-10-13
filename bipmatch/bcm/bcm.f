c=============================== BCM code ===============================
C BIPARTITE MATCHING   bm.f   Frank Chang and Tom McCormick
C comparison version: all lower case statement can be removed after tests.
C
      PARAMETER(M1=65600, N1=65600, L1=2500000)
c      PARAMETER(M1=5000, N1=5000, L1=2500000)
C M1, N1, L1 = POSSIBLE NUMBERS OF ROWS, COLUMNS, AND NON-ZEROS
C              IN A MATRIX, USED ONLY FOR DECLARING ARRAYS.
C THE INPUT ROUTINE "INPUT" WILL TAKE AS THE INPUT THREE INTEGERS AND
C TWO ARRAYS, THESE SHOULD BE PREPARED BY THE USER IN A FILE "cirp.dat"
C M = NUMBER OF ROWS
C N = NUMBER OF COLUMNS
C R = NUMBER OF NON-ZEROS
C CI: COLUMN-INDEX ARRAY
C RP: ROW-POINTER ARRAY
C
C OUTPUT:
C CM: ARRAY OF COLUMN-MATES.  "CM(J)=I" MEANS ROW I MATCHES TO COLUMN J.
C     IF CM(J)=0, COLUMN J IS FREE.
C MN: MATCHING NUMBER. IN OUTPUT IT'S THE SIZE OF THE MAX MATCHING.
C RL: ARRAY OF ROW-LABELS.  "RL(I).NE.0" MEANS ROW I IS LABELED.
C STACK: IN OUTPUT THE PART OF THE STACK FROM PCP TO M1 CONTAINS THE
C        LABELED ROWS.
C
      INTEGER   RUNTIME
      real      rr
      real      coeff(L1), tarray(2), tt
      integer   cheap, search, extend
c     eq: edge queries; eex: edge exchanges (op counts)
      integer   eq, eex
      INTEGER   RP(M1), M, N, R
      INTEGER   ARP(M1), NEXTC(M1)
      INTEGER   CI(L1), CM(N1), RL(M1), MN
      INTEGER   Q(M1/2), QP, STACK(M1), SP, PCP, TCP
      external  BeginTiming !$pragma C( BeginTiming )
      external  EndTiming !$pragma C( EndTiming )
      external  SampleTime !$pragma C( SampleTime )
      COMMON /CIRP/ M, N, R, RP, CI
      eex=0
      eq=0
      cheap=0
      search=0
      extend=0
c      tt=dtime(tarray)
      CALL INPUT
c      tt=dtime(tarray)
c      tt=tarray(1)+tarray(2)
c      write(*,*)'Input time =',tt
      Call BeginTiming()
C ............ FIRST PASS: find a maximal matching .............
      QP = 0
      MN = 0
      DO 20 I = 1, M
         II = RP(I)
         IEND=RP(I+1)
         IF (II-IEND) 15, 20, 20
  10     IF (II.GE.IEND) THEN
            QP = QP + 1
            Q(QP) = I
            ARP(I) = II
            GOTO 20
         ENDIF
  15     J = CI(II)
         IF (CM(J).LE.0) THEN
            CM(J) = I
            MN = MN + 1
            ARP(I) = II + 1
            GOTO 20
         ELSE
            II = II + 1
            GOTO 10
         ENDIF
  20  CONTINUE
      write(*,*)'initial matching size = ', MN
C ...... SECOND PASS: try to match the remaining free rows .......
      PCP = M1
      TCP = PCP
c change: unmatched vertices processed in stack order; unchanged again.
      DO 200 IQ = QP, 1, -1
c      DO 200 IQ = 1, QP
         I = Q(IQ)
         SP = 1
         STACK(1) = I
         RL(I) = -1
         K = I
C TRY TO FIND A CHEAP ASSIGNMENT IN THE CURRENT ROW K
  30     II = ARP(K)
         eq = eq + 1
         IF (II.GE.RP(K+1)) GOTO 55
  40     J = CI(II)
c         cheap = cheap + 1
         IF (CM(J).EQ.0) THEN
            ARP(K) = II+ 1
            GOTO 100
         ELSE
            eq = eq + 1
            II = II + 1
            IF (II-RP(K+1)) 40, 50, 50
         ENDIF
C NO CHEAP ASSIGNMENT
  50     ARP(K) = RP(K+1)
  55     NEXTC(K) = RP(K)
C TRY TO FIND A GOOD NEXTC(K) TO EXTEND THE SEARCH PATH
  60     II = NEXTC(K)
  70     IEND=RP(K+1)
         eq = eq + 1
  75     IF (II.GE.IEND) GOTO 80
         J = CI(II)
c         search = search + 1
         IF (RL(CM(J)).EQ.0) THEN
            GOTO 90
         ELSE
            II = II + 1
            eq = eq + 1
            GOTO 75
         ENDIF
C CANNOT EXTEND SEARCH PATH FROM THE CURRENT ROW K,
C BACKTRACK THE PATH (STACK)
  80     SP = SP - 1
         STACK(TCP) = K
         TCP = TCP - 1
         IF (SP.LE.0) THEN
            PCP = TCP
            GOTO 200
         ENDIF
C GET THE TOP ROW FROM STACK
         K = STACK(SP)
         GOTO 60
C FOUND A NEW ROW TO EXTEND THE PATH (PUT IT ON STACK)
  90     NEXTC(K) = II + 1
         K = CM(J)
         RL(K) = J
         SP = SP + 1
c         extend = extend + 1
         STACK(SP) = K
         GOTO 30
C REASSIGNMENT
 100     CM(J) = K
         MN = MN + 1
         DO 110 II = 1, SP-1
            J = RL(STACK(II+1))
            CM(J) = STACK(II)
            RL(STACK(II)) = 0
            eex = eex + 1
 110     CONTINUE
         RL(K) = 0

 115     if (tcp.lt.pcp) then
            tcp = tcp + 1
            rl(stack(tcp)) = 0
            goto 115
         endif

c         DO WHILE (TCP.LT.PCP)
c            TCP = TCP + 1
c            RL(STACK(TCP)) = 0
c         END DO
 200  CONTINUE
c      tt=dtime(tarray)
c      tt=(tarray(1)+tarray(2))*100.0
c      RUNTIME = EndTiming()/100.0
      RUNTIME = EndTiming()
c      WRITE (*,*) ' MAXIMUM TRANSVERSAL:'
c      WRITE (*,*) (CM(J), J = 1, N)
c      WRITE (*,*)'*(BCM) MATCHING # = ',MN
c      WRITE (*,900) tt
c 900  FORMAT('bcm match time (milisec) = ', e8.3)
c      write(*,910) RUNTIME
c 910  FORMAT('run time = ',I12,' hsecs')
c      den=(R*100.0)/(M*N*1.0)
c      write(20,999) M, N, R, den, tt, MN
c999   format(I8,I8,I8,f8.2, f8.2, I8)
c      IF (MN.LT.M.AND.MN.LT.N) WRITE (*,*) ' STRUCTURALLY SINGULAR!'
c      write(*,*)'*BCM...............'
c      write(*,9999) MN, cheap, search, extend
      rr = R
      write(*,9998) MN, (eq+eex)/1000, eq/rr
 9998 format('size= ', I5, ' opcount(th) ',I6,' , eq/r = ',F5.1)
c      write(20,9999) MN, cheap, search, extend
c9999  format('size=',I8,'cheap=',I12,'search=',I12,'extend=',I12)

      STOP
      END
C
      SUBROUTINE INPUT
      PARAMETER(M1=65600, N1=65600, L1=2500000)
c      PARAMETER(M1=5000, N1=5000, L1=2500000)
      real      coeff(L1)
      INTEGER   RP(M1), M, N, R
      INTEGER   CI(L1)
      COMMON /CIRP/ M, N, R, RP, CI
c      OPEN(9, FILE='cirp.dat', STATUS='OLD')
c      REWIND 9
c      READ (9,*) M, N, R
      READ (*,*) M, N, R
c      READ (9,*) (coeff(I), I= 1, R)
c      READ (9,*) (CI(I), I= 1, R)
      READ (*,*) (CI(I), I= 1, R)
c      READ (9,*) (RP(I), I= 1, M+1)
      READ (*,*) (RP(I), I= 1, M+1)
      RETURN
      END
c============================ end ====================================


