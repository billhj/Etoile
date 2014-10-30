h=figure;
%each dim q qd qdd
disp('draw fig for q qd qdd-----------');
% for i = 1 : 12
%     plot(model.q{1,i}, 'r');
%     hold on
%     plot(model.qd{1,i}, 'g');
%     plot(model.qdd{1,i}, 'b');
%     name = strcat(num2str(i), model.jtype{i});
%     name = strcat(name,'.fig');
%     saveas(h, name,'fig');
%     clf
% end
disp('end draw fig for q qd qdd----------');

%torque regression
disp('torque regression-----------');
% for i = 1 : 12
%     q1 = model.q{1,i};
%     q2 = model.qd{1,i};
%     x = [q1', q2'];
%     tau1 = model.tau{1,i};
%     y = tau1';
%     mdl = LinearModel.fit(x,y);
%     mdl
% end

for i = 7 : 7
    q1 = model.q{1,i};
    q2 = model.qd{1,i};
     tau0 = model.tau{1,4};
     q10 = model.q{1,1};
     q20 = model.qd{1,1};
      x = [q1', q2', tau0',q10', q20'];
%      x = [q1', q2'];
    tau1 = model.tau{1,i};
    y = tau1';
    mdl = LinearModel.fit(x,y);
    mdl
    plot(tau1, 'red');
    hold on
    plot(mdl.Fitted, 'blue');
end

disp('end torque regression-----------');