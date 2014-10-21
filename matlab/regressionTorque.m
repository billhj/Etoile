q1 = model.q{1,1};
q2 = model.qd{1,1};
x = [q1', q2'];
tau1 = model.tau{1,1};
y = tau1';
mdl = LinearModel.fit(x,y);
mdl
