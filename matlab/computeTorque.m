function [model] = computeTorque(model)
disp('computeTorque');
framesize = size(model.q, 2);
dof = size(model.q, 1);
%model.tau = zeros(1,dof);
model.NB = 12;

model.tau = zeros(size(model.q));


for j = 1 : framesize
    q = zeros(1,dof);
    qd = zeros(1,dof);
    qdd = zeros(1,dof);
%     qddo = zeros(1,dof);
    for n = 1 : dof
         q(n) = model.q(n,j);
         qd(n) = model.qd(n,j);
         qdd(n) = model.qdd(n,j);
    end
    disp(q);
    tau = ID(model, q, qd, qdd);
%     qdnext = qd + qdd /30;
%     qnext = q + qd / 30;
%     disp(qnext);
%     qddo = FDab(model, q, qd,tau);
    for n = 1 : dof
         model.tau(n,j) = tau(n,1);
    end
end

disp('end computeTorque');
end