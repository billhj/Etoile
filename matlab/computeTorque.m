function [model] = computeTorque(model)
disp('computeTorque');
framesize = size(model.q{1},2);
dof = size(model.q, 2);
%model.tau = zeros(1,dof);
model.NB = 12;
for n = 1 : dof
    model.tau{1, n} = zeros(1,framesize);
end

for j = 1 : framesize
    q = zeros(1,dof);
    qd = zeros(1,dof);
    qdd = zeros(1,dof);
    for n = 1 : dof
         q(n) = model.q{1,n}(1,j);
         qd(n) = model.qd{1,n}(1,j);
         qdd(n) = model.qdd{1,n}(1,j);
    end
    tau = ID(model, q, qd, qdd);
    for n = 1 : dof
         model.tau{1,n}(1,j) = tau(n,1);
    end
end

disp('end computeTorque');
end